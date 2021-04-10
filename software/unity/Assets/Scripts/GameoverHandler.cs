using System.Collections;
using UnityEngine;
using TMPro;
using UnityEngine.EventSystems;

public class GameoverHandler : EventTrigger
{
    public GameObject mWinStatus;
    public GameObject mExitText;
    private bool mReadyToQuit;

    /* Start is called before the first frame update */
    void Start()
    {
        string result = PlayerPrefs.GetString("result");
        string statusMessage;
        mReadyToQuit = false;

        switch (result)
        {
            case "win":
                statusMessage = "You win! \n\nAll enemy ships have sunk!";
                break;
            case "loss":
                statusMessage = "Nice Try! \n\nAll your ships have sunk!";
                break;
            case "playerForfeit":
                statusMessage = "Nice try!\n\nYou surrendered";
                break;
            case "opponentForfeit":
                statusMessage = "You win! \n\nYour opponent surrendered";
                break;
            default:
                statusMessage = "";
                break;
        }

        mWinStatus.GetComponent<TextMeshProUGUI>().text = statusMessage;
        StartCoroutine(EnableTapToExit());
    }

    /* Once player has tapped the gameover screen, close Unity and return to android app */
    public override void OnPointerUp(PointerEventData eventData)
    {
        base.OnPointerUp(eventData);
        if (mReadyToQuit)
        {
            Application.Quit();
        }
    }

    private IEnumerator EnableTapToExit()
    {
        /* Wait 3 seconds */
        yield return new WaitForSeconds(2);

        /* Show tap to exit message */
        TextMeshProUGUI exitMsg = mExitText.GetComponent<TextMeshProUGUI>();
        exitMsg.text = "Tap anywhere to exit";
        StartCoroutine(FadeTextToFullAlpha(1f, exitMsg));
        mReadyToQuit = true;
    }

    /* Text blinking subroutines */
    /* https://forum.unity.com/threads/fading-in-out-gui-text-with-c-solved.380822/ */
    public IEnumerator FadeTextToFullAlpha(float t, TextMeshProUGUI i)
    {
        i.color = new Color(i.color.r, i.color.g, i.color.b, 0);
        while (i.color.a < 1.0f)
        {
            i.color = new Color(i.color.r, i.color.g, i.color.b, i.color.a + (Time.deltaTime / t));
            yield return null;
        }
        StartCoroutine(FadeTextToZeroAlpha(2f, i));
    }

    public IEnumerator FadeTextToZeroAlpha(float t, TextMeshProUGUI i)
    {
        i.color = new Color(i.color.r, i.color.g, i.color.b, 1);
        while (i.color.a > 0.0f)
        {
            i.color = new Color(i.color.r, i.color.g, i.color.b, i.color.a - (Time.deltaTime / t));
            yield return null;
        }
        StartCoroutine(FadeTextToFullAlpha(1f, i));
    }
}
