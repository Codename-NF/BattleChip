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
                rectTransform.anchoredPosition = new Vector2((x * 70) + 50, (y * 70) + 50);

                // Call the cell's setup function
                mAllCells[x, y] = newCell.GetComponent<Cell>();
                mAllCells[x, y].Setup(new Vector2Int(x, y), this);
            }
        }

        for (int x = 0; x < 10; x += 2)
        {
            for (int y = 0; y < 10; y++)
            {
                int offset = (y % 2 != 0) ? 0 : 1;
                int finalX = x + offset;

                mAllCells[finalX, y].GetComponent<Image>().color = new Color32(79, 123, 159, 255);
            }
        }
    }
    /*
    public CellState ValidateCell(int targetx, int targetY, BasePiece checkingPiece)
    {
        Debug.Log("TODO: Implement Validation");
        return CellState.Empty;
    }
    */
}
