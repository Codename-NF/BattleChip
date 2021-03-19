using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum ShotType {
    None,
    Miss,
    Hit,
}

public class Board : MonoBehaviour
{
    public GameManager mGameManager;
    public GameObject mCellPrefab;
    public bool mDoneSetup = false;

    // 2D array of all cells in the Board
    public Cell[,] mAllCells = new Cell[10, 10];

    // 2D arrays to keep track of shots
    public ShotType[,] mShotsOnMe = new ShotType[10, 10];
    public ShotType[,] mShotsOnOpponent = new ShotType[10, 10];

    // Reference to the cell currently being targetted
    public Cell mTargetedCell;

    public void Create()
    {
        // Create a 10x10 board of cells
        for (int x = 0; x < 10; x++)
        {
            for (int y = 0; y < 10; y++)
            {
                // Create the cell
                GameObject newCell = Instantiate(mCellPrefab, transform);

                // Place the cell by taking it's (x,y) and scaling it to the screen
                RectTransform rectTransform = newCell.GetComponent<RectTransform>();
                rectTransform.anchoredPosition = new Vector2((x * 95) + 15, (y * 95) + 50);

                // Call the cell's setup function
                mAllCells[x, y] = newCell.GetComponent<Cell>();
                mAllCells[x, y].Setup(new Vector2Int(x, y), this);
            }
        }
        
        // Initialize arrays of shots taken to empty
        for (int x = 0; x < 10; x++)
        {
            for (int y = 0; y < 10; y++)
            {
                mShotsOnMe[x, y] = ShotType.None;
                mShotsOnOpponent[x, y] = ShotType.None;
            }
        }

        mTargetedCell = null;
        mDoneSetup = true;
    }

    public bool ValidateShips()
    {
        // Don't validate until ships have been placed
        if (!mDoneSetup) return false;

        // Check each cell in the 10x10 board
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                // If cell is occupied by more than one ship
                if (mAllCells[i, j].mCurrentPieces.Count > 1)
                {
                    return false;
                }
            }
        }
        // By this point, there are no cells with overlapping ships
        return true;
    }
}
