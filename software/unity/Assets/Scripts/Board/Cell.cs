using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

/* Enumerated states that a cell can be in */
public enum CellState
{
    /* General states */
    Default,
    Occupied,

    /* When two ships occupy the same tile */
    Stacked,

    /* When the player chooses to shoot this tile */
    Targeted,

    /* When shot at */
    Missed,
    Hit,
    Sunk,
}

/* A coordinate on the board, possibly containing part of a ship */
public class Cell : EventTrigger
{
    /* References to the assets used to render the cell */
    public Image mImage;
    private Sprite mRegularSprite;
    private Sprite mSunkSprite;

    /* State of the cell */
    public CellState mCellState;

    /* References to the cell's position, parent board, and placement vector */
    [HideInInspector]
    public Vector2Int mBoardPosition = Vector2Int.zero;
    [HideInInspector]
    public Board mBoard = null;
    [HideInInspector]
    public RectTransform mRectTransform = null;
    [HideInInspector]
    
    /* List of ships (usually 1) occupying the cell */
    public List<Ship> mIncludedShips;

    /* Flag used to decide whether the contained ship should be dragged or rotated on contact */
    private bool mWasDragged;

    /* Initialize the cell on the board in its respective position */
    public void Setup(Vector2Int newBoardPosition, Board newBoard)
    {
        mBoard = newBoard;
        mBoardPosition = newBoardPosition;
        mIncludedShips = new List<Ship>();
        mWasDragged = false;
        mRectTransform = GetComponent<RectTransform>();

        /* Load the cell's possible image components */
        mRegularSprite = Resources.Load<Sprite>("tileshape");
        mSunkSprite = Resources.Load<Sprite>("tileshape_sunk");
    }

    /* Update the cell's state (and colour) */
    protected void Update()
    {
        int xCoord = mBoardPosition.x;
        int yCoord = mBoardPosition.y;
        Color32 tempColor;

        /* If the cell is being tapped by the player, set it as the board's targeted cell */
        if (RectTransformUtility.RectangleContainsScreenPoint(mRectTransform, Input.mousePosition) &&
            !GlobalState.WaitingForPush && !GlobalState.GameIsPaused)
        {
            mBoard.mTargetedCell = this;
        }

        #region CellStateLogic
        /* Possible cell states when attacking opponent's board */
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
        /* Possible cell states when watching your own board being attacked */
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
        /* Possible cell states when initially placing your pieces */
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
        /* Update the colour of the cell based on it's state */
        mImage.sprite = mRegularSprite;
        switch (mCellState)
        {
            case CellState.Occupied: /* One ship on cell */
                mImage.color = GlobalState.ColorTheme.CellPieceColor;
                break;
            case CellState.Stacked: /* (Illegal) overlapping of ships */
                mImage.color = GlobalState.ColorTheme.CellOverlapColor;
                mImage.sprite = mSunkSprite;
                break;
            case CellState.Targeted: /* Player wants to shoot this cell */
                mImage.color = new Color32(255, 255, 255, 255);
                break;
            case CellState.Hit: /* This cell was shot (and hit a ship) */
                if (GlobalState.GameState == GameState.Attacking)
                {
                    /* Cell shows a hit on the opponent */
                    tempColor = GlobalState.ColorTheme.CellEnemyColor;
                    tempColor.a = 100;
                }
                else 
                {
                    /* Cell shows a hit on the player */
                    tempColor = GlobalState.ColorTheme.CellPieceColor;
                    tempColor.a = 100;
                }
                mImage.color = tempColor;
                break;
            case CellState.Sunk: /* This cell was shot (and hit a ship) */
                if (GlobalState.GameState == GameState.Attacking)
                {
                    /* Cell shows a hit on the opponent */
                    tempColor = GlobalState.ColorTheme.CellEnemyColor;
                    tempColor.a = 100;
                }
                else 
                {
                    /* Cell shows a hit on the player */
                    tempColor = GlobalState.ColorTheme.CellPieceColor;
                    tempColor.a = 100;
                    
                }
                mImage.color = tempColor;
                mImage.sprite = mSunkSprite;
                break;
            case CellState.Missed: /* This cell was shot (but had no ship) */
                mImage.color = new Color32(0, 0, 0, 0);
                break;
            default: /* Original checkerboard pattern */
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
    /* Logic performed by cell when it detects that the player finished clicking on it */
    public override void OnPointerUp(PointerEventData eventData)
    {
        base.OnPointerUp(eventData);
        /* If we are in the setup phase of the game */
        if (GlobalState.GameState == GameState.Placement)
        {
            /* If the ship wasn't dragged and we aren't waiting for a message from the DE1-S0C */
            if (!mWasDragged && !GlobalState.WaitingForPush)
            {
                /* Rotate the ship */
                mIncludedShips[0].Rotate(this);
            }
            /* Deselect the targeted ship */
            mBoard.mTargetedShip = null;
            mWasDragged = false;
        }
    }

    /* When cell detects that the player began a click on it */
    public override void OnPointerDown(PointerEventData eventData)
    {
        base.OnPointerDown(eventData);

        /* If we aren't waiting for communication from DE1-S0C */
        if (!GlobalState.WaitingForPush)
        {
            /* If we are in the placement/setup stage of the game */
            if (GlobalState.GameState == GameState.Placement)
            {
                /* Select a ship (if any) on the cell for dragging */
                mBoard.mOriginalCell = this;
                mBoard.mTargetedShip = (mIncludedShips.Count > 0) ? mIncludedShips[0] : null;
                mWasDragged = false;
            }
            /* If we are currently attacking */
            else if (GlobalState.GameState == GameState.Attacking)
            {
                /* If cell hasn't been shot yet, target it */
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
        /* Only allow dragging of ships during placement phase of the game */
        if (GlobalState.GameState == GameState.Placement && !GlobalState.WaitingForPush)
        {
            /* Indicate the the has begun dragging this cell */
            mWasDragged = true;

            /* Compute the difference in position between the old cell and new cell */
            int xOriginal = mBoard.mOriginalCell.mBoardPosition.x;
            int yOriginal = mBoard.mOriginalCell.mBoardPosition.y;
            int xNew = mBoard.mTargetedCell.mBoardPosition.x;
            int yNew = mBoard.mTargetedCell.mBoardPosition.y;
            int xDelta = xNew - xOriginal;
            int yDelta = yNew - yOriginal;

            /* If player dragged the cursor to a new tile */
            if ((xDelta != 0 || yDelta != 0) && mBoard.mTargetedShip != null)
            {
                /* If ship was moved successfully */
                if (mBoard.mTargetedShip.Move(xDelta, yDelta))
                {
                    mBoard.mOriginalCell = mBoard.mTargetedCell;
                }
            }
        }
    }
}
