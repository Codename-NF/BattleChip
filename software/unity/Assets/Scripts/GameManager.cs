using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;
public class GameManager : MonoBehaviour
{
    public Board mBoard;
    public PieceManager mPieceManager;
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
        mBoard.Create();
        mPieceManager.Setup(mBoard);
    }

    public void ConfirmButton()
    {
        AndroidJavaClass jc = new AndroidJavaClass("com.nf.battlechip.UnityMessage");

        switch (GlobalState.GameState)
        {
            case GameState.Placement:
                // SendBluetoothMessageToConsole(mPieceManager.ExportShips());
                string exportString = mPieceManager.ExportShips();
                Debug.Log(exportString); // Debug, Placement
                mStatus.GetComponent<TextMeshProUGUI>().text = "Placements confirmed. Waiting for opponent...";

                // CALL placement(exported_string)
                jc.CallStatic("placement", exportString);

                /*
                mTitle.GetComponent<TextMeshProUGUI>().text = "Your turn to attack!";
                mStatus.GetComponent<TextMeshProUGUI>().text = "Tap the tile you want to strike.";
                GlobalState.GameState = GameState.Attacking;
                */
                mWaiting = true;
                
                break;
            case GameState.Attacking:
                int xCoord = mBoard.mTargetedCell.mBoardPosition.x;
                int yCoord = mBoard.mTargetedCell.mBoardPosition.y;

                Debug.Log("Shoot " + xCoord.ToString() + " " + yCoord.ToString() + "\n");

                // CALL shoot(x,y)
                jc.CallStatic("shoot", xCoord, yCoord);

                mWaiting = true;
                break;
            // This will be removed once pushes from android are implemented
            case GameState.Defending:
                Debug.Log("Not implemented yet");
                mTitle.GetComponent<TextMeshProUGUI>().text = "Your turn to attack!";
                mStatus.GetComponent<TextMeshProUGUI>().text = "Tap the tile you want to strike.";
                GlobalState.GameState = GameState.Attacking;
                break;
            default:
                Debug.Log("ERROR: Button should be NOT visible right now!");
                break;
        }
    }

    #region Communication
    // Basic Communication
    void ReceiveBluetoothMessageFromConsole(string message)
    {
        Debug.Log("Received: " + message);
        // TODO: REPLACE THIS WITH ACTUAL PROCESSING
        SendBluetoothMessageToConsole(message);
    }

    void SendBluetoothMessageToConsole(string message)
    {
        Debug.Log("Sending: " + message);
        AndroidJavaClass jc = new AndroidJavaClass("com.nf.battlechip.activity.MainUnityActivity");
        AndroidJavaObject unityActivity = jc.GetStatic<AndroidJavaObject>("instance");
        unityActivity.Call("sendBluetoothMessageToConsole", message);
    }

    // Android Studio to Unity functions (Android studio calls these)
    void AndroidToUnity(string message)
    {
        Debug.Log("RECEIVED:\n" + message);
        string[] msgTokens = message.Split(' ');
        switch(msgTokens[0])
        {
            case "result": // result(int, int, bool)
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

                mWaiting = false;
                break;

            case "gameStart":
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
                break;
            default:
                break;
        }
    }
    #endregion
}
