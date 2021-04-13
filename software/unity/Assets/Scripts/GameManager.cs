using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;
using System.Threading.Tasks;

/* Object keeping track of the client's game state */
public class GameManager : MonoBehaviour
{
    /* Pieces to be added to the board on initialization */
    public static int[] ShipPieces = { 2, 3, 3, 4, 5 };

    public Board mBoard;
    public GameObject mTitle;
    public GameObject mStatus;
    public List<Ship> mShips;
    public ParticleSystem mBackground;

    private int mPlayerShipsRemaining;
    private bool mAllowStatusUpdates;
    private bool mIsGettingResult;

    /* Start is called before the first frame update */
    void Start()
    {
        mAllowStatusUpdates = false;
        mIsGettingResult = false;
        GlobalState.GameState = GameState.Placement;
        GlobalState.ColorTheme = new ColorTheme();
        GlobalState.WaitingForPush = false;
        GlobalState.GameIsPaused = false;

        /* Get color theme from android */
        long color = new AndroidJavaClass("com.nf.battlechip.activity.MainUnityActivity").CallStatic<long>("getColor");
        Debug.LogFormat("Color retrieved is {0}.", color);

        /* If no color is available, set red default */
        color = (color != 0) ? color : 4293467747; /* This is red */

        /* Extract color values from long variable */
        byte opacity = (byte) ((color & 0xFF000000) >> 24);
        byte red = (byte) ((color & 0x00FF0000) >> 16);
        byte blue = (byte) ((color & 0x0000FF00) >> 8);
        byte green = (byte) ((color & 0x000000FF));

        /* Generate board colors based on theme color */
        byte redLite = (byte) ((red >> 3) + 140);
        byte blueLite = (byte) ((blue >> 3) + 140);
        byte greenLite = (byte) ((green >> 3) + 140);

        byte redTint = (byte)((red >> 4) + 170);
        byte blueTint = (byte)((blue >> 4) + 170);
        byte greenTint = (byte)((green >> 4) + 170);

        /* Apply color theme for ships and board */
        GlobalState.ColorTheme.CellPieceColor = new Color32(red, blue, green, opacity);
        GlobalState.ColorTheme.CellColorDark = new Color32(redLite, blueLite, greenLite, opacity);
        GlobalState.ColorTheme.CellColorLight = new Color32(redTint, blueTint, greenTint, opacity);

        GlobalState.ColorTheme.CellOverlapColor = GlobalState.ColorTheme.CellPieceColor;
        GlobalState.ColorTheme.CellOverlapColor.a = 120;

        /* Enable the background particle emissions */
        var main = mBackground.main;
        main.startColor = new ParticleSystem.MinMaxGradient(new Color32(55, 57, 62, 255), new Color32(15, 17, 22, 255));
        var emission = mBackground.emission;
        emission.enabled = true;

        /* Create the board*/
        mBoard.Create();

        /* Put pieces (ships) onto the board */
        mShips = new List<Ship>();
        for (int i = 0; i < ShipPieces.Length; i++)
        {
            Debug.LogFormat("Creating ship of length {0} at {1}, {2}", ShipPieces[i], 0, i * 2);
            mShips.Add(Ship.CreateShip(mBoard, 0, i * 2, ShipPieces[i]));
        }

        mPlayerShipsRemaining = mShips.Count;
        mAllowStatusUpdates = true;
    }

    /* If any ships overlap, update the status message */
    private void Update()
    {
        if (mAllowStatusUpdates && !mBoard.ValidateShips() && GlobalState.GameState == GameState.Placement)
        {
            mAllowStatusUpdates = false; /* Only run one of these threads at a time */
            StartCoroutine(UpdatePlacementMessage());
        }
    }

    /* Logic used by the confirm button based on GameManager's state */
    public void ConfirmButton()
    {
        AndroidJavaClass jc = new AndroidJavaClass("com.nf.battlechip.interfaces.UnityMessage");

        switch (GlobalState.GameState)
        {
            case GameState.Placement:
                string exportString = CreatePlacementString(mShips);
                Debug.Log(exportString);
                mStatus.GetComponent<TextMeshProUGUI>().text = "Placements confirmed. Waiting for opponent...";

                /* Send ship placements to Android */
                jc.CallStatic("placement", exportString);

                GlobalState.WaitingForPush = true;
                break;

            case GameState.Attacking:
                int xCoord = mBoard.mTargetedCell.mBoardPosition.x;
                int yCoord = mBoard.mTargetedCell.mBoardPosition.y;

                Debug.Log("Shoot " + xCoord.ToString() + " " + yCoord.ToString() + "\n");
                mStatus.GetComponent<TextMeshProUGUI>().text = "Firing at " + Convert.ToChar(xCoord + 65) + (yCoord + 1).ToString() + "...";

                /* Send target coords to Android */
                jc.CallStatic("shoot", xCoord, yCoord);

                GlobalState.WaitingForPush = true;
                break;

            /* No button logic for Defending because there is nothing for the camera to confirm */
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
            case "start":
                if (GlobalState.GameState == GameState.Placement && GlobalState.WaitingForPush)
                {
                    /* If player is first */
                    if (int.Parse(msgTokens[1]) == 1)
                    {
                        /* Prepare attack screen */
                        mTitle.GetComponent<TextMeshProUGUI>().text = "Your Turn to Attack!";
                        mStatus.GetComponent<TextMeshProUGUI>().text = "Tap the tile you want to strike.";
                        GlobalState.GameState = GameState.Attacking;

                        GlobalState.WaitingForPush = false;
                    }
                    else
                    {
                        /* Prepare defending screen */
                        mTitle.GetComponent<TextMeshProUGUI>().text = "Opponent's Turn...";
                        mStatus.GetComponent<TextMeshProUGUI>().text = "Your opponent is attacking!";
                        GlobalState.GameState = GameState.Defending;

                        GlobalState.WaitingForPush = true;
                    }
                }
                break;

            case "result":
                if (GlobalState.GameState == GameState.Attacking && GlobalState.WaitingForPush)
                {
                    mIsGettingResult = true;
                    int xCoord = int.Parse(msgTokens[1]);
                    int yCoord = int.Parse(msgTokens[2]);
                    int gameStatus = int.Parse(msgTokens[3]); /* 1 == gameover */
                    int hitStatus = int.Parse(msgTokens[4]); /* 0 == miss, 1 == hit, 2 == sunk */
                    mBoard.mTargetedCell = null;

                    /* Update the player's history of shots on the opponent */
                    switch (hitStatus)
                    {
                        case 0:
                            mStatus.GetComponent<TextMeshProUGUI>().text = "Missed! The shot did not connect!";
                            mBoard.mShotsOnOpponent[xCoord, yCoord] = ShotType.Miss;
                            break;
                        case 1:
                            mStatus.GetComponent<TextMeshProUGUI>().text = "Success! You hit a ship!";
                            mBoard.mShotsOnOpponent[xCoord, yCoord] = ShotType.Hit;
                            break;
                        case 2:
                            mStatus.GetComponent<TextMeshProUGUI>().text = "Nice! You sunk a ship!";
                            /* Get the ship's position, update each of it's cells to 'sunk' */
                            
                            int shipX = int.Parse(msgTokens[5]);
                            int shipY = int.Parse(msgTokens[6]);
                            int shipL = int.Parse(msgTokens[7]);
                            int shipO = int.Parse(msgTokens[8]); /* Vertical == 1, Horizontal == 2 */
                            
                            for (int i = 0; i < shipL; i++)
                            {
                                /* Update cell to sunk */
                                mBoard.mShotsOnOpponent[shipX, shipY] = ShotType.Sunk;
                                if (shipO == 2)
                                {
                                    shipX++;
                                }
                                else
                                {
                                    shipY++;
                                }
                            }
                            break;
                        default:
                            Debug.Log("Error: Bad hitStatus value");
                            break;
                    }
                    
                    if (gameStatus == 1) /* If player sunk the opponent's last ship */
                    {
                        StartCoroutine(EndTheGame("win"));
                    }
                    else /* Wait a moment, then switch state to opponent's turn */
                    {
                        StartCoroutine(ProcessPlayerAttackResult());
                    }
                }
                break;

            case "targeted":
                if (!mIsGettingResult)
                {
                    HandleTargeted(int.Parse(msgTokens[1]), int.Parse(msgTokens[2]));
                }
                else
                {
                    /* NOTE: Our hardware accelerated AI makes its decision SO FAST that we need to delay the handling of 'targeted' */
                    Task.Delay(3000).ContinueWith(t => AndroidToUnity("targeted " + msgTokens[1] + " " + msgTokens[2]));
                }
                
                break;
            case "f": /* Opponent forfeited */
                StartCoroutine(EndTheGame("opponentForfeit"));
                break;

            default:
                break;
        }
    }

    /* Handle Targeted message */
    private void HandleTargeted(int x, int y)
    {
        if (GlobalState.GameState == GameState.Defending && GlobalState.WaitingForPush)
        {
            int xCoord = x;
            int yCoord = y;

            /* Shot hit if the cell has 1 piece on it */
            bool didHit = (mBoard.mAllCells[xCoord, yCoord].mIncludedShips.Count == 1);

            /* Update list of shots */
            mBoard.mShotsOnMe[xCoord, yCoord] = (didHit) ? ShotType.Hit : ShotType.Miss;

            if (didHit)
            {
                /* If the connecting shot sinks the ship */
                if (mBoard.mAllCells[xCoord, yCoord].mIncludedShips[0].HasShipSunk())
                {
                    mStatus.GetComponent<TextMeshProUGUI>().text = "Oof! Our ship at " + Convert.ToChar(xCoord + 65) + (yCoord + 1).ToString() + " has sunk!";
                    mPlayerShipsRemaining--;
                }
                else
                {
                    mStatus.GetComponent<TextMeshProUGUI>().text = "Ouch! Our ship at " + Convert.ToChar(xCoord + 65) + (yCoord + 1).ToString() + " has been hit!";
                }
            }
            else
            {
                mStatus.GetComponent<TextMeshProUGUI>().text = "Whew! Your opponent missed!";
            }

            if (mPlayerShipsRemaining < 1)
            {
                StartCoroutine(EndTheGame("loss"));
            }
            else
            {
                /* Wait a moment, then switch state to player's turn */
                StartCoroutine(ProcessOpponentAttack());
            }

        }
    }
    /* Display the opponent's shot on player, wait three seconds, the swap to attack */
    private IEnumerator ProcessOpponentAttack()
    {
        /* Wait 3 seconds */
        yield return new WaitForSeconds(3);
        
        /* Prepare attack screen */
        mTitle.GetComponent<TextMeshProUGUI>().text = "Your Turn to Attack!";
        mStatus.GetComponent<TextMeshProUGUI>().text = "Tap the tile you want to strike.";
        GlobalState.GameState = GameState.Attacking;

        GlobalState.WaitingForPush = false;
    }

    /* Display the opponent's shot on player, wait three seconds, the swap to attack */
    private IEnumerator ProcessPlayerAttackResult()
    {
        /* Wait 3 seconds */
        yield return new WaitForSeconds(3);

        /* Prepare defending screen */
        mTitle.GetComponent<TextMeshProUGUI>().text = "Opponent's Turn...";
        mStatus.GetComponent<TextMeshProUGUI>().text = "Your opponent is attacking!";
        GlobalState.GameState = GameState.Defending;

        GlobalState.WaitingForPush = true;
        mIsGettingResult = false; /* Done getting result */
    }

    private IEnumerator UpdatePlacementMessage()
    {
        /* Wait a second */
        yield return new WaitForSeconds(1);

        /* If there is still a collision, update status; */
        if (!mBoard.ValidateShips() && GlobalState.GameState == GameState.Placement)
        {
            mStatus.GetComponent<TextMeshProUGUI>().text = "Make sure no ships overlap!";
        }
        else
        {
            mStatus.GetComponent<TextMeshProUGUI>().text = "Press confirm when ready";
        }
        mAllowStatusUpdates = true; /* Allow another one of these threads to run */
    }

    private IEnumerator EndTheGame(string result)
    {
        /* Wait two seconds */
        yield return new WaitForSeconds(2);

        PlayerPrefs.SetString("result", result);
        SceneManager.LoadScene(1); /* Gameover scene has index 1 */
    }
}
