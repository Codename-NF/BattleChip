using UnityEngine;
using UnityEngine.SceneManagement;

/* Pause menu, toggled by the Android backbutton */
public class PauseMenu : MonoBehaviour
{
    public GameObject pauseMenuUI;

    /* Update is called once per frame */
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if (GlobalState.GameIsPaused)
            {
                Resume();
            }
            else
            {
                Pause();
            }
        }
    }

    /* Close pause menu */
    public void Resume()
    {
        pauseMenuUI.SetActive(false);
        GlobalState.GameIsPaused = false;
    }

    /* Open pause menu */
    void Pause()
    {
        pauseMenuUI.SetActive(true);
        GlobalState.GameIsPaused = true;
    }

    /* Forfeit the game */
    public void Forfeit()
    {
        Debug.Log("Forfeiting Game");
        AndroidJavaClass jc = new AndroidJavaClass("com.nf.battlechip.interfaces.UnityMessage");
        jc.CallStatic("forfeit");

        PlayerPrefs.SetString("result", "playerForfeit");
        SceneManager.LoadScene(1); /* Gameover scene has index 1 */
    }
}
