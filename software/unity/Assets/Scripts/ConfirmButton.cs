using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ConfirmButton : MonoBehaviour
{
    public Board mBoard;
    public PieceManager mPieceManager;
    
    // Start is called before the first frame update
    void Start()
    {
        GetComponent<Button>().interactable = false;
    }

    void FixedUpdate()
    {
        GetComponent<Button>().interactable = (mPieceManager.mNoActiveInteraction && mBoard.ValidateShips());
    }
}
