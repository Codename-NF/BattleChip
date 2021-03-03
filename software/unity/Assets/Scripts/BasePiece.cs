using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class BasePiece : EventTrigger
{
    public Image mOutlineImage;

    [HideInInspector]
    public Color mColor = Color.clear;

    private Cell mLastCell = null;
    private Cell mCurrentCell = null;

    private RectTransform mRectTransform = null;
    private PieceManager mPieceManager;

    private Cell mTargetCell = null;

    private Vector3Int mMovement = Vector3Int.one;
    private List<Cell> mHighlightedCells = new List<Cell>();

    private int mShipSize;
    private bool mIsHorizontal;
    private bool mWasDragged;

    // Convert the ship to a string, which we will send to the server over bluetooth
    public string ExportShip()
    {
        string shipString;

        shipString = mCurrentCell.mBoardPosition.x.ToString();
        shipString += " ";
        shipString += mCurrentCell.mBoardPosition.y.ToString();
        shipString += " ";
        shipString += mShipSize;
        shipString += " ";
        shipString += mIsHorizontal;
        shipString += "\n";

        Debug.Log("Exporting ship as: " + shipString);

        return shipString;
    }

    public void Setup(Color32 newSpriteColor, PieceManager newPieceManager, int Size)
    {
        mPieceManager = newPieceManager;

        GetComponent<Image>().color = newSpriteColor;
        mRectTransform = GetComponent<RectTransform>();

        mShipSize = Size;
        mIsHorizontal = true;
    }

    public void Place(Cell newCell)
    {
        // Cell stuff
        mCurrentCell = newCell;
        mLastCell = newCell;
        mCurrentCell.mCurrentPieces.Add(this);

        // Object stuff
        transform.position = newCell.transform.position;
        gameObject.SetActive(true);

        MoveTail(mCurrentCell, mIsHorizontal, true, mShipSize);
    }

    private void Move()
    {
        // Clear current
        mCurrentCell.mCurrentPieces.Remove(this);

        // Switch cells
        mCurrentCell = mTargetCell;
        mCurrentCell.mCurrentPieces.Add(this);

        // Move on board
        transform.position = mCurrentCell.transform.position;
        mTargetCell = null;

        MoveTail(mCurrentCell, mIsHorizontal, true, mShipSize);
    }

    public override void OnBeginDrag(PointerEventData eventData)
    {
        base.OnBeginDrag(eventData);

        mWasDragged = true;
    }

    public override void OnDrag(PointerEventData eventData)
    {
        base.OnDrag(eventData);

        // Follow pointer
        transform.position += (Vector3)eventData.delta;

        // Check for overlapping available squares
        
        // For each available cell
        foreach (Cell cell in mCurrentCell.mBoard.mAllCells)
        {
            // Check if cell is available and cursor is in that available cell
            if (cell.mCurrentPieces.Count == 0 && RectTransformUtility.RectangleContainsScreenPoint(cell.mRectTransform, Input.mousePosition))
            {
                // If the mouse is within a valid cell, get it, and break.
                mTargetCell = cell;
                break;
            }

            // We do not move the piece if the cursor is not within a valid cell
            mTargetCell = null;
        }
        
    }

    public override void OnEndDrag(PointerEventData eventData)
    {
        base.OnEndDrag(eventData);
        Board board = mCurrentCell.mBoard;

        // If we haven't chosen a cell when releasing the piece, put it back to where it was
        if (!mTargetCell || !VerifyPlacement(mTargetCell, mIsHorizontal, mShipSize))
        {
            transform.position = mCurrentCell.gameObject.transform.position;
            mTargetCell = mCurrentCell;
            MoveTail(mCurrentCell, mIsHorizontal, true, mShipSize);
        }
        else
        {
            // Move to new cell
            Move();
        }
    }

    public override void OnPointerDown(PointerEventData eventData)
    {
        base.OnPointerDown(eventData);
        // Signal that the user is currently updating a piece's location/orientation
        mPieceManager.mNoActiveInteraction = false;

        MoveTail(mCurrentCell, mIsHorizontal, false, mShipSize);
        mWasDragged = false;
    }

    public override void OnPointerUp(PointerEventData eventData)
    {
        base.OnPointerUp(eventData);
        Cell endCell = (mTargetCell) ? mTargetCell : mCurrentCell;
        if (!mWasDragged)
        {
            if (VerifyPlacement(endCell, !mIsHorizontal, mShipSize))
            {
                mIsHorizontal = !mIsHorizontal;
            }
            MoveTail(endCell, mIsHorizontal, true, mShipSize);
        }
        // Signal that the user is not currently moving any pieces
        mPieceManager.mNoActiveInteraction = true;
    }

    private void MoveTail(Cell anchorCell, bool horizontal, bool placing, int length)
    {
        Cell cellToUpdate;
        Board board = anchorCell.mBoard;

        int xCoord = anchorCell.mBoardPosition.x;
        int yCoord = anchorCell.mBoardPosition.y;

        for (int i = 0; i < length - 1; i++)
        {
            // Get coords of cell to update
            if (horizontal)
            {
                xCoord++;
            }
            else
            {
                yCoord--;
            }

            // Add this piece to the cell
            cellToUpdate = board.mAllCells[xCoord, yCoord];

            if (placing)
            {
                cellToUpdate.mCurrentPieces.Add(this);
            }
            else
            {
                cellToUpdate.mCurrentPieces.Remove(this);
            }
        }
    }
    
    private bool VerifyPlacement(Cell location, bool isHorizontal, int shipLength)
    {
        bool valid = true;
        int biggestX = (isHorizontal) ? location.mBoardPosition.x + shipLength - 1 : location.mBoardPosition.x;
        int smallestY = (!isHorizontal) ? location.mBoardPosition.y - shipLength + 1 : location.mBoardPosition.y;

        if (biggestX >= 10 || smallestY < 0)
        {
            valid = false;
        }

        return valid;
    }
}
