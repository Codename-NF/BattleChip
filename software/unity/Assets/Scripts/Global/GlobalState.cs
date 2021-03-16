using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum GameState
{
    Placement,
    Attacking,
    Defending,
};

public static class GlobalState
{
    // Global state of the Unity application 
    private static ColorTheme colorTheme; // = new ColorTheme();
    private static GameState gameState;


    public static GameState GameState
    {
        get
        {
            return gameState;
        }
        set
        {
            gameState = value;
        }
    }

    public static ColorTheme ColorTheme
    {
        get
        {
            return colorTheme;
        }
        set
        {
            colorTheme = value;
        }
    }

    // Singleton logic, only one instance of the state can be active
    /*
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
    } */

    // Load global data when object is instantiated on new scene
    /*
    void Start()
    {
        savedColorTheme = GlobalState.Instance.savedColorTheme;
        GameState = GameState.Placement;
    }*/

    // Save copy of state before switching scenes
    /*
    public void SaveState()
    {
        GlobalState.Instance.savedColorTheme = savedColorTheme;
    } */
}
