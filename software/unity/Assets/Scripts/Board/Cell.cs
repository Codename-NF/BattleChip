using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Cell : MonoBehaviour
{
    public GlobalState Instance;
    public Image mImage;

    [HideInInspector]
    public Vector2Int mBoardPosition = Vector2Int.zero;
    [HideInInspector]
    public Board mBoard = null;
    [HideInInspector]
    public RectTransform mRectTransform = null;

    
    [HideInInspector]
    public List<BasePiece> mCurrentPieces = new List<BasePiece>();
    
    // Attach Cell to the board, and assign its position (relative to 10x10)
    public void Setup(Vector2Int newBoardPosition, Board newBoard)
    {
        mBoard = newBoard;
        mBoardPosition = newBoardPosition;

        mRectTransform = GetComponent<RectTransform>();
    }

    protected void Update()
    {
        // If cell has a piece colour that cell
        switch (mCurrentPieces.Count)
        {
            case 0:
                // Otherwise, give it the original colour (checkerboard)
                if (mBoardPosition.x % 2 != mBoardPosition.y % 2)
                {
                    mImage.color = Instance.savedColorTheme.CellColorLight;
                }
                else
                {
                    mImage.color = Instance.savedColorTheme.CellColorDark;
                }
                break;
            case 1:
                mImage.color = Instance.savedColorTheme.CellPieceColor;
                break;
            default:
                mImage.color = Instance.savedColorTheme.CellOverlapColor;
                break;
        }
    }
}
