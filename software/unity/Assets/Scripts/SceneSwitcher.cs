using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneSwitcher : MonoBehaviour
{
    public void goToHome()
    {
        SceneManager.LoadScene(0);
    }

    public void goToPlacement()
    {
        SceneManager.LoadScene(1);
    }
}
