using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Cell : MonoBehaviour
{
    public Image mOutlineImage;

    [HideInInspector]
    public Vector2Int mBoardPosition = Vector2Int.zero;
    [HideInInspector]
    public Board mBoard = null;
    [HideInInspector]
    public RectTransform mRectTransform = null;

    
    [HideInInspector]
    public BasePiece mCurrentPiece = null;
    
    // Attach Cell to the board, and assign its position (relative to 10x10)
    public void Setup(Vector2Int newBoardPosition, Board newBoard)
    {
        mBoard = newBoard;
        mBoardPosition = newBoardPosition;

        mRectTransform = GetComponent<RectTransform>();
    }
}
