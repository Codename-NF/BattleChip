using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/* Possible states that the game can be in */
public enum GameState
{
    Placement,
    Attacking,
    Defending,
};

/* Static object holding the state of the application */
public static class GlobalState
{
    private static ColorTheme colorTheme;
    private static GameState gameState;
    private static bool waitingForPush;
    private static bool gameIsPaused;

    /* Keeps track of the games state (placement/attack/defend) */
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

    /* Keeps track of the imported color scheme */
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

    /* Keeps track of when Unity is waiting for a response */
    public static bool WaitingForPush
    {
        get
        {
            return waitingForPush;
        }
        set
        {
            waitingForPush = value;
        }
    }

    /* Tracks if Unity UI is in the pause/forfeit menu */
    public static bool GameIsPaused
    {
        get
        {
            return gameIsPaused;
        }
        set
        {
            gameIsPaused = value;
        }
    }
}
