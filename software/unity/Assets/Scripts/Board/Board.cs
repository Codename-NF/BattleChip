using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum CellState
{
    Empty,
    Occupied,
    OutOfBounds
}

public class Board : MonoBehaviour
{
    public GameObject mCellPrefab;
    public bool mDoneSetup = false;

    // 2D array of all cells in the Board
    public Cell[,] mAllCells = new Cell[10, 10];

    public void Create()
    {
        Debug.Log("Create Board Called!");
        // Create a 10x10 board of cells
        for (int x = 0; x < 10; x++)
        {
            for (int y = 0; y < 10; y++)
            {
                // Create the cell
                GameObject newCell = Instantiate(mCellPrefab, transform);

                // Place the cell by taking it's (x,y) and scaling it to the screen
                RectTransform rectTransform = newCell.GetComponent<RectTransform>();
                rectTransform.anchoredPosition = new Vector2((x * 80) + 5, (y * 80) + 50);

                // Call the cell's setup function
                mAllCells[x, y] = newCell.GetComponent<Cell>();
                mAllCells[x, y].Setup(new Vector2Int(x, y), this);
            }
        }
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
