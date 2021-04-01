using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class GameManager : MonoBehaviour
{
    // Pieces to be added to the board on initialization
    public static int[] ShipPieces = { 2, 3, 3, 4, 5 };

    public Board mBoard;
    public GameObject mTitle;
    public GameObject mStatus;
    public List<Ship> mShips;

    // Start is called before the first frame update
    void Start()
    {
        GlobalState.GameState = GameState.Placement;
        GlobalState.ColorTheme = new ColorTheme();
        GlobalState.WaitingForPush = false;
        GlobalState.GameIsPaused = false;

        // Get color theme from android        
        long color = new AndroidJavaClass("com.nf.battlechip.activity.MainUnityActivity").CallStatic<long>("getColor");
        Debug.LogFormat("Color retrieved is {0}.", color);

        // If no color is available, set as default
        color = (color != 0) ? color : 4284612846; // purple

        // Extract color values from long variable
        byte opacity = (byte) ((color & 0xFF000000) >> 24);
        byte red = (byte) ((color & 0x00FF0000) >> 16);
        byte blue = (byte) ((color & 0x0000FF00) >> 8);
        byte green = (byte) ((color & 0x000000FF));

        // Generate board colors based on theme color
        byte redLite = (byte) ((red >> 3) + 80);
        byte blueLite = (byte) ((blue >> 3) + 80);
        byte greenLite = (byte) ((green >> 3) + 80);

        byte redTint = (byte)((red >> 4) + 120);
        byte blueTint = (byte)((blue >> 4) + 120);
        byte greenTint = (byte)((green >> 4) + 120);

        // Apply color theme directly to ship
        GlobalState.ColorTheme.CellPieceColor = new Color32(red, blue, green, opacity);
        GlobalState.ColorTheme.CellColorDark = new Color32(redLite, blueLite, greenLite, opacity);
        GlobalState.ColorTheme.CellColorLight = new Color32(redTint, blueTint, greenTint, opacity);

        // Create a board and put pieces on it
        mBoard.Create();

        mShips = new List<Ship>();
        for (int i = 0; i < ShipPieces.Length; i++)
        {
            // Ship.CreateShip(Board board, int xCoord, int yCoord, int length)
            Debug.LogFormat("Creating ship of length {0} at {1}, {2}", ShipPieces[i], 0, i * 2);
            mShips.Add(Ship.CreateShip(mBoard, 0, i * 2, ShipPieces[i]));
        }
    }

    // Logic used by the confirm button based on GameManager's state
    public void ConfirmButton()
    {
        AndroidJavaClass jc = new AndroidJavaClass("com.nf.battlechip.UnityMessage");

        switch (GlobalState.GameState)
        {
            case GameState.Placement:
                string exportString = CreatePlacementString(mShips);
                Debug.Log(exportString); // Debug, Placement
                mStatus.GetComponent<TextMeshProUGUI>().text = "Placements confirmed. Waiting for opponent...";

                // Send ship placements to Android
                jc.CallStatic("placement", exportString);

                GlobalState.WaitingForPush = true;
                break;

            case GameState.Attacking:
                int xCoord = mBoard.mTargetedCell.mBoardPosition.x;
                int yCoord = mBoard.mTargetedCell.mBoardPosition.y;

                Debug.Log("Shoot " + xCoord.ToString() + " " + yCoord.ToString() + "\n");
                mStatus.GetComponent<TextMeshProUGUI>().text = "Firing at (" + xCoord.ToString() + ", " + yCoord.ToString() + ") ...";

                // Send target coords to Android
                jc.CallStatic("shoot", xCoord, yCoord);

                GlobalState.WaitingForPush = true;
                break;

            // No button logic for Defending because there is nothing for the camera to confirm

            default:
                Debug.Log("ERROR: Button should be NOT visible right now!");
                break;
        }
    }

    /* Get string representing ship placements */
    private string CreatePlacementString(List<Ship> shipsToExport)
    {
        string exportString = "";
        for (int i = 0; i < shipsToExport.Count; i++)
        {
            if (i > 0)
            {
                exportString += " ";
            }
            exportString += shipsToExport[i].ExportShip();
        }
        return exportString;
    }

    /* Android Studio to Unity functions (Android studio calls these) */
    public void AndroidToUnity(string message)
    {
        Debug.Log("RECEIVED:\n" + message);
        string[] msgTokens = message.Split(' ');
        switch(msgTokens[0])
        {
            case "gameStart":
                if (GlobalState.GameState == GameState.Placement && GlobalState.WaitingForPush)
                {
                    // If player is first
                    if (bool.Parse(msgTokens[1]))
                    {
                        // Prepare attack screen
                        mTitle.GetComponent<TextMeshProUGUI>().text = "Your turn to attack!";
                        mStatus.GetComponent<TextMeshProUGUI>().text = "Tap the tile you want to strike.";
                        GlobalState.GameState = GameState.Attacking;

                        GlobalState.WaitingForPush = false;
                    }
                    else
                    {
                        // Prepare defending screen
                        mTitle.GetComponent<TextMeshProUGUI>().text = "Opponent's turn...";
                        mStatus.GetComponent<TextMeshProUGUI>().text = "Your opponent attacking!";
                        GlobalState.GameState = GameState.Defending;

                        GlobalState.WaitingForPush = true;
                    }
                }
                break;

            case "result":
                if (GlobalState.GameState == GameState.Attacking && GlobalState.WaitingForPush)
                {
                    int xCoord = int.Parse(msgTokens[1]);
                    int yCoord = int.Parse(msgTokens[2]);
                    bool didHit = bool.Parse(msgTokens[3]);

                    // Update board with shot status
                    mBoard.mShotsOnOpponent[xCoord, yCoord] = (didHit) ? ShotType.Hit : ShotType.Miss;
                    mBoard.mTargetedCell = null;

                    if (didHit)
                    {
                        mStatus.GetComponent<TextMeshProUGUI>().text = "Success! You hit a ship!";
                    }
                    else
                    {
                        mStatus.GetComponent<TextMeshProUGUI>().text = "Missed! The shot did not connect!";
                    }

                    // Wait a moment, then switch state to opponent's turn
                    StartCoroutine(ProcessPlayerAttackResult());
                }
                break;

            case "targeted":
                if (GlobalState.GameState == GameState.Defending && GlobalState.WaitingForPush)
                {
                    int xCoord = int.Parse(msgTokens[1]);
                    int yCoord = int.Parse(msgTokens[2]);

                    // Shot hit if the cell has 1 piece on it
                    bool didHit = (mBoard.mAllCells[xCoord, yCoord].mIncludedShips.Count == 1);

                    // Update list of shots
                    mBoard.mShotsOnMe[xCoord, yCoord] = (didHit) ? ShotType.Hit : ShotType.Miss;

                    if (didHit)
                    {
                        // If the connecting shot sinks the ship
                        if (mBoard.mAllCells[xCoord, yCoord].mIncludedShips[0].HasShipSunk())
                        {
                            mStatus.GetComponent<TextMeshProUGUI>().text = "Oof! Our ship at (" + xCoord + ", " + yCoord + ") has sunk!";
                        }
                        else
                        {
                            mStatus.GetComponent<TextMeshProUGUI>().text = "Ouch! Out ship at (" + xCoord + ", " + yCoord + ") has been hit!";
                        }
                    }
                    else
                    {
                        mStatus.GetComponent<TextMeshProUGUI>().text = "Whew! Your opponent missed!";
                    }

                    // Wait a moment, then switch state to player's turn
                    StartCoroutine(ProcessOpponentAttack());
                }
                break;

            default:
                break;
        }
    }

    /* Display the opponent's shot on player, wait three seconds, the swap to attack */
    private IEnumerator ProcessOpponentAttack()
    {
        // Wait 3 seconds
        yield return new WaitForSeconds(3);
        
        // Prepare attack screen
        mTitle.GetComponent<TextMeshProUGUI>().text = "Your turn to attack!";
        mStatus.GetComponent<TextMeshProUGUI>().text = "Tap the tile you want to strike.";
        GlobalState.GameState = GameState.Attacking;

        GlobalState.WaitingForPush = false;
    }

    /* Display the opponent's shot on player, wait three seconds, the swap to attack */
    private IEnumerator ProcessPlayerAttackResult()
    {
        // Wait 3 seconds
        yield return new WaitForSeconds(3);

        // Prepare defending screen
        mTitle.GetComponent<TextMeshProUGUI>().text = "Opponent's turn...";
        mStatus.GetComponent<TextMeshProUGUI>().text = "Your opponent attacking!";
        GlobalState.GameState = GameState.Defending;

        GlobalState.WaitingForPush = true;
    }
}
