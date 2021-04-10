using UnityEngine;
using UnityEngine.UI;
using TMPro;

/* Logic bound to the confirm button to decide when it's rendered/enabled */
public class ConfirmButton : MonoBehaviour
{
    public GameManager mGameManager;
    public Board mBoard;
    
    /* Initialize the button to unclickable */
    void Start()
    {
        GetComponent<Button>().interactable = false;
    }

    /* Logic to decide when to enable/render this confirm button */
    void FixedUpdate()
    {
        switch (GlobalState.GameState)
        {
            case GameState.Placement:
                /* Make confirm button visible */
                GetComponent<Image>().enabled = true;
                GetComponentInChildren<TextMeshProUGUI>().enabled = true;
                
                /* Enable button if no ships overlap and the player hasn't confirmed yet */
                GetComponent<Button>().interactable = mBoard.ValidateShips() && !GlobalState.WaitingForPush;
                break;
            case GameState.Attacking:
                /* Make confirm button visible */
                GetComponent<Image>().enabled = true;
                GetComponentInChildren<TextMeshProUGUI>().enabled = true;
                
                /* Enable button if the player has targeted a cell but hasn't confirmed yet */
                GetComponent<Button>().interactable = (mBoard.mTargetedCell != null) && !GlobalState.WaitingForPush;
                break;
            case GameState.Defending:
                /* Hide confirm button */
                GetComponent<Image>().enabled = false;
                GetComponentInChildren<TextMeshProUGUI>().enabled = false;
                break;
            default:
                Debug.Log("ERROR: This should not be possible!");
                break;
        }
    }
}
