using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public Board mBoard;
    public PieceManager mPieceManager;
    
    // Start is called before the first frame update
    void Start()
    {
        switch (SceneManager.GetActiveScene().buildIndex)
        {
            case 0: // Placement
                mBoard.Create();
                mPieceManager.Setup(mBoard);
                break;
            case 1:
                mBoard.Create();
                break;
            default:
                Debug.Log("This scene shouldn't have a GameManager");
                break;
        }
        
    }

    void ReceiveBluetoothMessageFromConsole(string message) {
        Debug.Log("Received: " + message);
        // TODO: REPLACE THIS WITH ACTUAL PROCESSING
    }

    void SendPlacementMessage(string message) {
        Debug.Log("Sending: " + message);
        AndroidJavaClass jc = new AndroidJavaClass("com.nf.battlechip.UnityMessage");
        jc.CallStatic("placement", message);
    }

    public void ConfirmButton()
    {
        switch (SceneManager.GetActiveScene().buildIndex)
        {
            case 0: // Placement
                SendPlacementMessage(mPieceManager.ExportShips());
                break;
            case 1:
                
                break;
            default:
                Debug.Log("Error, this button shouldn't exist on this scene");
                break;
        }
    }
}
