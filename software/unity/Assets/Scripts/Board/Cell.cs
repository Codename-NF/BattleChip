using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public enum CellState
{
    Default,
    Occupied,
    Stacked,
    Targeted,
    Missed,
    Hit,
}

public class Cell : EventTrigger
{
    public Image mImage;
    public CellState mCellState;

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
        int xCoord = mBoardPosition.x;
        int yCoord = mBoardPosition.y;

        // Update State
        // Debug.Log( (int) mBoard.mGameManager.mGameState);
        // Possible cell states when attacking opponent's board
        if (GlobalState.GameState == GameState.Attacking)
        {
            if (mBoard.mShotsOnOpponent[xCoord, yCoord] == ShotType.Hit)
            {
                mCellState = CellState.Hit;
            }
            else if (mBoard.mShotsOnOpponent[xCoord, yCoord] == ShotType.Miss)
            {
                mCellState = CellState.Missed;
            }
            else if (mBoard.mTargetedCell == this)
            {
                mCellState = CellState.Targeted;
            }
            else
            {
                mCellState = CellState.Default;
            }
        }
        // Possible cell states when watching your own board being attacked
        else if (GlobalState.GameState == GameState.Defending)
        {
            if (mBoard.mShotsOnMe[xCoord, yCoord] == ShotType.Hit)
            {
                mCellState = CellState.Hit;
            }
            else if (mBoard.mShotsOnMe[xCoord, yCoord] == ShotType.Miss)
            {
                mCellState = CellState.Missed;
            }
            else if (mCurrentPieces.Count == 1)
            {
                mCellState = CellState.Occupied;
            }
            else
            {
                mCellState = CellState.Default;
            }
        }
        // Possible cell states when initially placing your pieces
        else
        {
            if (mCurrentPieces.Count > 1)
            {
                mCellState = CellState.Stacked;
            }
            else if (mCurrentPieces.Count == 1)
            {
                mCellState = CellState.Occupied;
            }
            else
            {
                mCellState = CellState.Default;
            }
        }
        
        // If cell has a piece colour that cell
        switch (mCellState)
        {
            case CellState.Occupied: // One ship on cell
                mImage.color = GlobalState.ColorTheme.CellPieceColor;
                break;
            case CellState.Stacked: // (Illegal) overlapping of ships
                mImage.color = GlobalState.ColorTheme.CellOverlapColor;
                break;
            case CellState.Targeted: // Player wants to shoot this cell
                mImage.color = new Color32(255, 255, 255, 255);
                break;
            case CellState.Hit: // This cell was shot (and hit a ship)
                mImage.color = GlobalState.ColorTheme.CellPieceColor;
                break;
            case CellState.Missed: // This cell was shot (but it was empty)
                mImage.color = new Color32(255, 255, 255, 10);
                break;
            default: // Original checkerboard color
                if (mBoardPosition.x % 2 != mBoardPosition.y % 2)
                {
                    mImage.color = GlobalState.ColorTheme.CellColorLight;
                }
                else
                {
                    mImage.color = GlobalState.ColorTheme.CellColorDark;
                }
                break;
        }
    }
    
    public override void OnPointerUp(PointerEventData eventData)
    {
        base.OnPointerUp(eventData);
        // If this cell was clicked while we are attacking
        if (GlobalState.GameState == GameState.Attacking &&
            RectTransformUtility.RectangleContainsScreenPoint(this.mRectTransform, Input.mousePosition) && 
            this.mCellState == CellState.Default)
        {
            // Replace the board's current target with this cell
            mBoard.mTargetedCell = this;
        }
    }
    /*
    public override void OnPointerDown(PointerEventData eventData)
    {
        base.OnPointerDown(eventData);

        Debug.Log("PointerDown");
    }*/
}
