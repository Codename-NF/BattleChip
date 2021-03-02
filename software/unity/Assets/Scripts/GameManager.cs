using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public Board mBoard;
    public PieceManager mPieceManager;
    
    // Start is called before the first frame update
    void Start()
    {
        Debug.Log(name);
        Debug.Log(mBoard + " " + mPieceManager);
        mBoard.Create();
        mPieceManager.Setup(mBoard);
    }

    void ReceiveBluetoothMessageFromConsole(string message) {
        Debug.Log("Received: " + message);
        // TODO: REPLACE THIS WITH ACTUAL PROCESSING
        SendBluetoothMessageToConsole(message);
    }

    void SendBluetoothMessageToConsole(string message) {
        Debug.Log("Sending: " + message);
        AndroidJavaClass jc = new AndroidJavaClass("com.nf.battlechip.MainUnityActivity");
        AndroidJavaObject unityActivity = jc.GetStatic<AndroidJavaObject>("instance");
        unityActivity.Call("sendBluetoothMessageToConsole", message);
    }

}
