using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GlobalState : MonoBehaviour
{
    public static GlobalState Instance;

    // Global state of the Unity application 
    public Color32 boardColor;
    public Color32 shipColor;

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
        boardColor = GlobalState.Instance.boardColor;
        shipColor = GlobalState.Instance.shipColor;
    }

    // Save copy of state before switching scenes
    public void SaveState()
    {
        GlobalState.Instance.boardColor = boardColor;
        GlobalState.Instance.shipColor = shipColor;
    }
}
