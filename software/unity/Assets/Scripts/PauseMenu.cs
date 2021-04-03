using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PauseMenu : MonoBehaviour
{
    public GameObject pauseMenuUI;

    // Update is called once per frame
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

    public void Resume()
    {
        pauseMenuUI.SetActive(false);
        // Time.timeScale = 1f; // Resumes time
        GlobalState.GameIsPaused = false;
    }

    void Pause()
    {
        pauseMenuUI.SetActive(true);
        // Time.timeScale = 0f; // Freezes time
        GlobalState.GameIsPaused = true;
    }

    public void Forfeit()
    {
        Debug.Log("Forfeiting Game");
        AndroidJavaClass jc = new AndroidJavaClass("com.nf.battlechip.UnityMessage");
        jc.CallStatic("forfeit");

        PlayerPrefs.SetString("result", "playerForfeit");
        SceneManager.LoadScene(1); // Gameover scene has index 1
    }
}
