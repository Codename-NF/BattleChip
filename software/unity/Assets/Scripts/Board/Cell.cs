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
    Sunk,
}

public class Cell : EventTrigger
{
    public Image mImage;
    private Sprite mRegularSprite;
    private Sprite mSunkSprite;

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
        mRegularSprite = Resources.Load<Sprite>("tileshape");
        mSunkSprite = Resources.Load<Sprite>("tileshape_sunk");
    }

    protected void Update()
    {
        int xCoord = mBoardPosition.x;
        int yCoord = mBoardPosition.y;
        Color32 tempColor;

        // Update the currently targeted cell (the cell that is being hovered over)
        if (RectTransformUtility.RectangleContainsScreenPoint(mRectTransform, Input.mousePosition) &&
            !GlobalState.WaitingForPush && !GlobalState.GameIsPaused)
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
            else if (mBoard.mShotsOnOpponent[xCoord, yCoord] == ShotType.Sunk)
            {
                mCellState = CellState.Sunk;
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
            else if (mBoard.mShotsOnMe[xCoord, yCoord] == ShotType.Sunk)
            {
                mCellState = CellState.Sunk;
            }
            else if (mBoard.mShotsOnMe[xCoord, yCoord] == ShotType.Miss)
            {
                mCellState = CellState.Missed;
            }
            else if (mIncludedShips.Count == 1)
            {
                mCellState = CellState.Occupied;
            }
            else if (mIncludedShips.Count > 1) 
            {
                // This shouldn't occur on the defence screen
                // This is here for debugging purposes
                mCellState = CellState.Targeted;
            }
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
            else
            {
                mCellState = CellState.Default;
            }
        }
        #endregion

        #region CellColorLogic
        // If cell has a piece colour that cell
        mImage.sprite = mRegularSprite;
        switch (mCellState)
        {
            case CellState.Occupied: // One ship on cell
                mImage.color = GlobalState.ColorTheme.CellPieceColor;
                break;
            case CellState.Stacked: // (Illegal) overlapping of ships
                mImage.color = GlobalState.ColorTheme.CellOverlapColor;
                mImage.sprite = mSunkSprite;
                break;
            case CellState.Targeted: // Player wants to shoot this cell
                mImage.color = new Color32(255, 255, 255, 255);
                break;
            case CellState.Hit: // This cell was shot (and hit a ship)
                if (GlobalState.GameState == GameState.Attacking)
                {
                    // Cell shows a hit on the opponent
                    tempColor = GlobalState.ColorTheme.CellEnemyColor;
                    tempColor.a = 100;
                }
                else 
                {
                    // Cell shows a hit on the player
                    tempColor = GlobalState.ColorTheme.CellPieceColor;
                    tempColor.a = 100;
                }
                mImage.color = tempColor;
                break;
            case CellState.Sunk: // This cell was shot (and hit a ship)
                if (GlobalState.GameState == GameState.Attacking)
                {
                    // Cell shows a hit on the opponent
                    tempColor = GlobalState.ColorTheme.CellEnemyColor;
                    tempColor.a = 100;
                }
                else 
                {
                    // Cell shows a hit on the player
                    tempColor = GlobalState.ColorTheme.CellPieceColor;
                    tempColor.a = 100;
                    
                }
                mImage.color = tempColor;
                mImage.sprite = mSunkSprite;
                break;
            case CellState.Missed: // This cell was shot (but it was empty)
                mImage.color = new Color32(0, 0, 0, 0);
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
        if (GlobalState.GameState == GameState.Placement)
        {
            if (!mWasDragged && !GlobalState.WaitingForPush)
            {
                mIncludedShips[0].Rotate(this);
            }
            mBoard.mTargetedShip = null;
            mWasDragged = false;
        }
    }

    public override void OnPointerDown(PointerEventData eventData)
    {
        base.OnPointerDown(eventData);
        if (!GlobalState.WaitingForPush)
        {
            if (GlobalState.GameState == GameState.Placement)
            {
                mBoard.mOriginalCell = this;
                mBoard.mTargetedShip = (mIncludedShips.Count > 0) ? mIncludedShips[0] : null;
                mWasDragged = false;
            }
            else if (GlobalState.GameState == GameState.Attacking)
            {
                if (mCellState == CellState.Default)
                {
                    mBoard.mTargetedCell = this;
                }
            }
        }
    }

    /* Cell detects dragging and moves ship when current cell != the cell hovered by the player */
    public override void OnDrag(PointerEventData eventData)
    {
        base.OnDrag(eventData);
        // Only allow dragging of ships during placement phase of the game
        if (GlobalState.GameState == GameState.Placement && !GlobalState.WaitingForPush)
        {
            mWasDragged = true;

            // Compute delta between old cell and new cell
            int xOriginal = mBoard.mOriginalCell.mBoardPosition.x;
            int yOriginal = mBoard.mOriginalCell.mBoardPosition.y;
            int xNew = mBoard.mTargetedCell.mBoardPosition.x;
            int yNew = mBoard.mTargetedCell.mBoardPosition.y;
            int xDelta = xNew - xOriginal;
            int yDelta = yNew - yOriginal;

            // Debug.LogFormat("Delta is {0},{1}", xDelta, yDelta);
            // If player dragged between two different tiles
            if ((xDelta != 0 || yDelta != 0) && mBoard.mTargetedShip != null)
            {
                // If ship was moved successfully
                if (mBoard.mTargetedShip.Move(xDelta, yDelta))
                {
                    Debug.Log("Ship was moved!");
                    mBoard.mOriginalCell = mBoard.mTargetedCell;
                }
            }
        }
    }
}
