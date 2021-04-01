using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ship
{
    /* Basic ship properties */
    private int mLength;
    private int mOrientation;
    private bool mHasSunk;

    /* Reference to the board, and cells the ship occupies */
    private Board mBoard;
    private List<Cell> mCells;

    /* Static constructor to create ships */
    public static Ship CreateShip(Board board, int xCoord, int yCoord, int length)
    {
        int xTemp = xCoord; // Don't need y since ships are initialized horizontally

        Ship newShip = new Ship();
        newShip.mLength = length;
        newShip.mOrientation = 2; // 1==Vertical, 2==Horizontal
        newShip.mHasSunk = false;
        newShip.mBoard = board;

        // Save the list of cells this new ship occupies and add this ship to each of them
        newShip.mCells = new List<Cell>();
        for (int i = 0; i < length; i++)
        {
            // Debug.LogFormat("Adding cell at {0},{1}", xTemp, yCoord);
            newShip.mCells.Add(board.mAllCells[xTemp, yCoord]);
            newShip.mCells[i].mIncludedShips.Add(newShip);
            // Debug.LogFormat("Added cell at {0},{1}", xTemp, yCoord);
            xTemp++;
        }

        return newShip;
    }

    /* Checks if the ship has sunk */
    public bool HasShipSunk()
    {
        int x, y;

        // Ship cannot be unsunk
        if (mHasSunk)
        {
            return true;
        }

        // If any part of the ship hasn't been hit, ship hasn't sunk yet
        for (int i = 0; i < mCells.Count; i++)
        {
            x = mCells[i].mBoardPosition.x;
            y = mCells[i].mBoardPosition.y;

            // Check if this tile has been shot
            if (mBoard.mShotsOnMe[x, y] != ShotType.Hit)
            {
                return false;
            }
        }

        // If all parts of the ship have been hit, sink the ship
        for (int j = 0; j < mCells.Count; j++)
        {
            x = mCells[j].mBoardPosition.x;
            y = mCells[j].mBoardPosition.y;
            mBoard.mShotsOnMe[x, y] = ShotType.Sunk;
        }
        mHasSunk = true;

        return true;
    }

    /* Translate ship (if possible) */
    public bool Move(int xDelta, int yDelta)
    {
        // First check if the ship's head would be in a valid location after translation
        int xNewHead = mCells[0].mBoardPosition.x + xDelta;
        int yNewHead = mCells[0].mBoardPosition.y + yDelta;
        int xNew = xNewHead;
        int yNew = yNewHead;

        // If head falls outside the board, do nothing and return false
        if (xNewHead < 0 || xNewHead > 9 ||
            yNewHead < 0 || yNewHead > 9)
        {
            Debug.LogFormat("New head positon at {0},{1} is invalid.", xNewHead, yNewHead);
            return false;
        }

        // If the rest of the ship cannot be moved to that position, do nothing and return false
        if (!IsValidOperation("place", mBoard.mAllCells[xNewHead, yNewHead]))
        {
            Debug.LogFormat("Movement Operation from {0},{1} to {2},{3} is invalid!", mCells[0].mBoardPosition.x, mCells[0].mBoardPosition.y, xNewHead, yNewHead);
            return false;
        }
        Debug.LogFormat("Confirmed that Movement operation from {0},{1} to {2},{3} is valid", mCells[0].mBoardPosition.x, mCells[0].mBoardPosition.y, xNewHead, yNewHead);
        
        // If we reached this point, then the move operation is valid
        // Remove this ship from each of it's current cells
        while (mCells.Count > 0)
        {
            mCells[0].mIncludedShips.Remove(this);
            mCells.RemoveAt(0);
        }
        Debug.Log("Done removing ship from old cells");
        // Add this ship to each of it's new cells
        while (mCells.Count < mLength)
        {
            mCells.Add(mBoard.mAllCells[xNew, yNew]); // Push onto back
            mCells[mCells.Count - 1].mIncludedShips.Insert(0, this); // Push onto front

            if (mOrientation == 2)
            {
                xNew++;
            }
            else
            {
                yNew--;
            }
        }
        Debug.Log("Done adding ship to new cells");

        // Operation successful, so we return true
        return true;
    }

    /* Rotate ship (if possible) */
    public bool Rotate(Cell mAnchorCell)
    {
        int xOldHead = mCells[0].mBoardPosition.x;
        int yOldHead = mCells[0].mBoardPosition.y;
        int xNew, yNew;

        if (!IsValidOperation("rotate", mAnchorCell))
        {
            return false;
        }

        // If we reach this point, the rotation is valid
        // Remove this ship from each of it's current cells
        while (mCells.Count > 0)
        {
            mCells[0].mIncludedShips.Remove(this);
            mCells.RemoveAt(0);
        }

        if (mOrientation == 2) // If horizontal
        {
            // Get coords of new head of the ship
            xNew = mAnchorCell.mBoardPosition.x;
            yNew = mAnchorCell.mBoardPosition.y + (mAnchorCell.mBoardPosition.x - xOldHead);

            // Add this ship to each of it's new cells
            while (mCells.Count < mLength)
            {
                mCells.Add(mBoard.mAllCells[xNew, yNew]); // Push onto back
                mCells[mCells.Count - 1].mIncludedShips.Insert(0, this); // Push onto front
                yNew--; // Decrement since ship head must be at the top
            }
            mOrientation = 1; // Now the ship is vertical
        }
        else // If vertical
        {
            xNew = mAnchorCell.mBoardPosition.x + (mAnchorCell.mBoardPosition.y - yOldHead);
            yNew = mAnchorCell.mBoardPosition.y;

            // Add this ship to each of it's new cells
            while (mCells.Count < mLength)
            {
                mCells.Add(mBoard.mAllCells[xNew, yNew]); // Push onto front
                mCells[mCells.Count - 1].mIncludedShips.Insert(0, this); // Push onto back
                xNew++;
            }
            mOrientation = 2; // Now the ship is horizontal
        }
        // Operation successful, so we return true
        return true;
    }

    /* Target cell is the new location of the ship's head node, or anchor for rotation */
    private bool IsValidOperation(string operation, Cell targetCell)
    {
        int xTarget = targetCell.mBoardPosition.x;
        int yTarget = targetCell.mBoardPosition.y;

        switch (operation)
        {
            case "place":
                // Check if the head is out of bounds
                if (xTarget < 0 || yTarget < 0 || xTarget > 9 || yTarget > 9)
                {
                    return false;
                }
                // Check if tail goes out of bounds
                if ((mOrientation == 2 && xTarget + mLength > 10) || // 1==Vertical, 2==Horizontal
                    (mOrientation == 1 && yTarget - mLength < -1))
                {
                    return false;
                }
                return true;

            case "rotate":
                // Flip vertically
                int xOld, yOld;
                int xNew, yNew;

                // For each tile in the ship's list
                for (int i = 0; i < mLength; i++)
                {
                    xOld = mCells[i].mBoardPosition.x;
                    yOld = mCells[i].mBoardPosition.y;

                    // Compute new coordinates
                    if (mOrientation == 2)
                    {
                        xNew = xTarget;
                        yNew = (xTarget - xOld) + yTarget;
                    }
                    else
                    {
                        xNew = (yTarget - yOld) + xTarget;
                        yNew = yTarget;
                    }

                    // If any new coordinate is invalid, then isValid returns false
                    if (xNew < 0 || xNew > 9 || yNew < 0 || yNew > 9)
                    {
                        return false;
                    }
                }
                return true;

            default:
                Debug.Log("Error: This operation does not exist");
                return false;
        }
    }

    /* Export ship data as a string */
    public string ExportShip()
    {
        string x = mCells[0].mBoardPosition.x + " ";
        string y = mCells[0].mBoardPosition.y + " ";
        string l = mLength + " ";
        string o = (mOrientation == 2) ? "2" : "1";

        return x + y + l + o;
    }
}
