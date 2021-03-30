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
    // public List<BasePiece> mCurrentPieces = new List<BasePiece>();
    public List<Ship> mIncludedShips;

    private bool mWasDragged;

    // Attach Cell to the board, and assign its position (relative to 10x10)
    public void Setup(Vector2Int newBoardPosition, Board newBoard)
    {
        mBoard = newBoard;
        mBoardPosition = newBoardPosition;
        mIncludedShips = new List<Ship>();
        mWasDragged = false;

        mRectTransform = GetComponent<RectTransform>();
    }

    protected void Update()
    {
        int xCoord = mBoardPosition.x;
        int yCoord = mBoardPosition.y;

        // Update the currently targeted cell (the cell that is being hovered over)
        if (RectTransformUtility.RectangleContainsScreenPoint(mRectTransform, Input.mousePosition))
        {
            mBoard.mTargetedCell = this;
        }

        #region CellStateLogic
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
            /*
            else if (mCurrentPieces.Count == 1)
            {
                mCellState = CellState.Occupied;
            }*/
            else
            {
                mCellState = CellState.Default;
            }
        }
        // Possible cell states when initially placing your pieces
        else
        {
            if (mIncludedShips.Count > 1)
            {
                mCellState = CellState.Stacked;
            }
            else if (mIncludedShips.Count == 1)
            {
                mCellState = CellState.Occupied;
            }
            else if (RectTransformUtility.RectangleContainsScreenPoint(mRectTransform, Input.mousePosition))
            {
                mCellState = CellState.Missed;
            }
            else
            {
                mCellState = CellState.Default;
            }
        }
        #endregion

        #region CellColorLogic
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
        #endregion
    }
    public override void OnPointerUp(PointerEventData eventData)
    {
        base.OnPointerUp(eventData);

        if (!mWasDragged)
        {
            mIncludedShips[0].Rotate(this);
        }
        mBoard.mTargetedShip = null;
        mWasDragged = false;
        /*
        // If this cell was clicked while we are attacking
        if (GlobalState.GameState == GameState.Attacking &&
            RectTransformUtility.RectangleContainsScreenPoint(this.mRectTransform, Input.mousePosition) && 
            this.mCellState == CellState.Default)
        {
            // Replace the board's current target with this cell
            mBoard.mTargetedCell = this;
        }
        mBoard.mOriginalCell = null; */
    }

    public override void OnPointerDown(PointerEventData eventData)
    {
        base.OnPointerDown(eventData);
        mBoard.mOriginalCell = this;
        mBoard.mTargetedShip = (mIncludedShips.Count > 0) ? mIncludedShips[0] : null;
        mWasDragged = false;
        // mBoard.mTargetedCell = this;
        // Debug.LogFormat("Cell {0},{1} detected a MousePointerDown event", mBoardPosition.x, mBoardPosition.y);
    }

    /* Cell detects dragging and moves ship when current cell != the cell hovered by the player */
    public override void OnDrag(PointerEventData eventData)
    {
        base.OnDrag(eventData);

        mWasDragged = true;
        // Only allow dragging of ships during placement phase of the game
        if (GlobalState.GameState == GameState.Placement)
        {
            // Compute delta between old cell and new cell
            int xOriginal = mBoard.mOriginalCell.mBoardPosition.x;
            int yOriginal = mBoard.mOriginalCell.mBoardPosition.y;
            int xNew = mBoard.mTargetedCell.mBoardPosition.x;
            int yNew = mBoard.mTargetedCell.mBoardPosition.y;
            int xDelta = xNew - xOriginal;
            int yDelta = yNew - yOriginal;

            Debug.LogFormat("Delta is {0},{1}", xDelta, yDelta);
            // If player dragged between two different tiles
            if (xDelta != 0 || yDelta != 0)
            {
                
                // If ship was moved successfully
                if (mBoard.mTargetedShip.Move(xDelta, yDelta))
                {
                    Debug.Log("Ship was moved!");
                    mBoard.mOriginalCell = mBoard.mTargetedCell;
                }
            }
            // Debug.LogFormat("OnDrag: {0}, {1}", mBoardPosition.x, mBoardPosition.y)
        }
    }
}
