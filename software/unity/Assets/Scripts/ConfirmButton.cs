using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

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
        switch (SceneManager.GetActiveScene().buildIndex)
        {
            case 0:
                GetComponent<Button>().interactable = (mPieceManager.mNoActiveInteraction && mBoard.ValidateShips());
                break;
            case 1:
                break;

            default:
                break;
        }
        
    }
}
