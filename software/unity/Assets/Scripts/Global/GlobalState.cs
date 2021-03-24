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
    private static ColorTheme colorTheme;
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
}
