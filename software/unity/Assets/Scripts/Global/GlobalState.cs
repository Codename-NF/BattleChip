using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GlobalState : MonoBehaviour
{
    public static GlobalState Instance;

    // Global state of the Unity application 
    public ColorTheme savedColorTheme = new ColorTheme();

    // Singleton logic, only one instance of the state can be active
    void Awake()
    {
        if (Instance == null)
        {
            DontDestroyOnLoad(gameObject);
            Instance = this;
        }
        else if (Instance != this)
        {
            Destroy(gameObject);
        }
    }

    // Load global data when object is instantiated on new scene
    void Start()
    {
        savedColorTheme = GlobalState.Instance.savedColorTheme;
    }

    // Save copy of state before switching scenes
    public void SaveState()
    {
        GlobalState.Instance.savedColorTheme = savedColorTheme;
    }
}
