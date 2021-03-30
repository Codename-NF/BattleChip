using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;
public class GameManager : MonoBehaviour
{
    // Pieces to be added to the board on initialization
    public static int[] ShipPieces = { 2, 3, 3, 4, 5 };

    public Board mBoard;
    // public PieceManager mPieceManager;
    public GameObject mTitle;
    public GameObject mStatus;
    public bool mWaiting;

    // public GameState mGameState;

    // Start is called before the first frame update
    void Start()
    {
        GlobalState.GameState = GameState.Placement;
        GlobalState.ColorTheme = new ColorTheme();
        mWaiting = false;
        
        // Create a board and put pieces on it
        mBoard.Create();

        for (int i = 0; i < ShipPieces.Length; i++)
        {
            // Ship.CreateShip(Board board, int xCoord, int yCoord, int length)
            Debug.LogFormat("Creating ship of length {0} at {1}, {2}", ShipPieces[i], 0, i * 2);
            Ship.CreateShip(mBoard, 0, i * 2, ShipPieces[i]);
        }
    }

    public void ConfirmButton()
    {
        AndroidJavaClass jc = new AndroidJavaClass("com.nf.battlechip.UnityMessage");

        switch (GlobalState.GameState)
        {
            case GameState.Placement:
                // string exportString = mPieceManager.ExportShips();
                // Debug.Log(exportString); // Debug, Placement
                mStatus.GetComponent<TextMeshProUGUI>().text = "Placements confirmed. Waiting for opponent...";

                // Send ship placements to Android
                // jc.CallStatic("placement", exportString);
                // jc.Call("getColor");

                mWaiting = true;
                break;

            case GameState.Attacking:
                int xCoord = mBoard.mTargetedCell.mBoardPosition.x;
                int yCoord = mBoard.mTargetedCell.mBoardPosition.y;

                Debug.Log("Shoot " + xCoord.ToString() + " " + yCoord.ToString() + "\n");

                // Send target coords to Android
                jc.CallStatic("shoot", xCoord, yCoord);

                mWaiting = true;
                break;

            // No button logic for Defending because there is nothing for the camera to confirm

            default:
                Debug.Log("ERROR: Button should be NOT visible right now!");
                break;
        }
    }

    // Basic Communication
    void ReceiveBluetoothMessageFromConsole(string message)
    {
        Debug.Log("Received: " + message);
        // TODO: REPLACE THIS WITH ACTUAL PROCESSING
    }

    void SendPlacementMessage(string message) {
        Debug.Log("Sending: " + message);
        AndroidJavaClass jc = new AndroidJavaClass("com.nf.battlechip.UnityMessage");
        jc.CallStatic("placement", message);
    }

    // Android Studio to Unity functions (Android studio calls these)
    void AndroidToUnity(string message)
    {
        Debug.Log("RECEIVED:\n" + message);
        string[] msgTokens = message.Split(' ');
        switch(msgTokens[0])
        {
            case "gameStart":
                if (GlobalState.GameState == GameState.Placement && mWaiting)
                {
                    // If player is first
                    if (bool.Parse(msgTokens[1]))
                    {
                        // Prepare attack screen
                        mTitle.GetComponent<TextMeshProUGUI>().text = "Your turn to attack!";
                        mStatus.GetComponent<TextMeshProUGUI>().text = "Tap the tile you want to strike.";
                        GlobalState.GameState = GameState.Attacking;

                    }
                    else
                    {
                        // Prepare defending screen
                        mTitle.GetComponent<TextMeshProUGUI>().text = "Opponent's turn...";
                        mStatus.GetComponent<TextMeshProUGUI>().text = "Your opponent is attacking your board!";
                        GlobalState.GameState = GameState.Defending;
                    }

                    mWaiting = false;
                }
                break;

            case "result":
                if (GlobalState.GameState == GameState.Attacking && mWaiting)
                {
                    int xCoord = int.Parse(msgTokens[1]);
                    int yCoord = int.Parse(msgTokens[2]);
                    bool didHit = bool.Parse(msgTokens[3]);

                    // Update board with shot status
                    mBoard.mShotsOnOpponent[xCoord, yCoord] = (didHit) ? ShotType.Hit : ShotType.Miss;
                    mBoard.mTargetedCell = null;

                    // TODO: Insert 2 second sleep here

                    // Prepare defending screen
                    mTitle.GetComponent<TextMeshProUGUI>().text = "Opponent's turn...";
                    mStatus.GetComponent<TextMeshProUGUI>().text = "Your opponent is attacking your board!";
                    GlobalState.GameState = GameState.Defending;

                    mWaiting = true;
                }
                break;

            case "targeted":
                if (GlobalState.GameState == GameState.Defending && mWaiting)
                {
                    int xCoord = int.Parse(msgTokens[1]);
                    int yCoord = int.Parse(msgTokens[2]);

                    // Shot hit if the cell has 1 piece on it
                    // bool didHit = (mBoard.mAllCells[xCoord, yCoord].mCurrentPieces.Count == 1);
                    bool didHit = true;
                    // Update list of shots
                    mBoard.mShotsOnMe[xCoord, yCoord] = (didHit) ? ShotType.Hit : ShotType.Miss;

                    // TODO: Insert 2 second sleep here

                    // Prepare attack screen
                    mTitle.GetComponent<TextMeshProUGUI>().text = "Your turn to attack!";
                    mStatus.GetComponent<TextMeshProUGUI>().text = "Tap the tile you want to strike.";
                    GlobalState.GameState = GameState.Attacking;

                    mWaiting = false;
                }
                break;

            default:
                break;
        }
    }
}
