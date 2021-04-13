using UnityEngine;

/* Possible states before/after a tile on the board has been shot at */
public enum ShotType {
    None,
    Miss,
    Hit,
    Sunk,
}

/* 2D array of cells, keeps track of where it has been hit */
public class Board : MonoBehaviour
{
    /* Reference to object containing client game logic */
    public GameManager mGameManager;
    public GameObject mCellPrefab;
    public bool mDoneSetup = false;

    /* 2D array of all cells in the Board */
    public Cell[,] mAllCells = new Cell[10, 10];

    /* 2D arrays to keep track of shots */
    public ShotType[,] mShotsOnMe = new ShotType[10, 10];
    public ShotType[,] mShotsOnOpponent = new ShotType[10, 10];

    /* References ship/cell actively targeted by player (for dragging operations) */
    public Cell mOriginalCell;
    public Cell mTargetedCell;
    public Ship mTargetedShip;

    /* Setup function to initialize the board and its cells */
    public void Create()
    {
        /* Create a 10x10 board of cells */
        for (int x = 0; x < 10; x++)
        {
            for (int y = 0; y < 10; y++)
            {
                /* Create the cell */
                GameObject newCell = Instantiate(mCellPrefab, transform);

                /* Place the cell onto the screen */
                RectTransform rectTransform = newCell.GetComponent<RectTransform>();
                rectTransform.anchoredPosition = new Vector2((x * 95) + 15, ((9 - y) * 95) + 50);

                /* Initialize the cell */
                mAllCells[x, y] = newCell.GetComponent<Cell>();
                mAllCells[x, y].Setup(new Vector2Int(x, y), this);
            }
        }
        
        /* Initialize the board's record of shots to empty 2D arrays */
        for (int x = 0; x < 10; x++)
        {
            for (int y = 0; y < 10; y++)
            {
                mShotsOnMe[x, y] = ShotType.None;
                mShotsOnOpponent[x, y] = ShotType.None;
            }
        }

        /* Initialize actively targeted ship/cell to null */
        mTargetedCell = null;
        mOriginalCell = null;
        mTargetedShip = null;

        /* Indicate that the board is done its initialization */
        mDoneSetup = true;
    }

    /* Returns true if no ships overlap, false otherwise */
    public bool ValidateShips()
    {
        /* Don't validate until ships have been placed */
        if (!mDoneSetup) return false;

        /* Check each cell in the 10x10 board */
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                /* If the cell is occupied by more than one ship */
                if (mAllCells[i, j].mIncludedShips.Count > 1)
                {
                    return false;
                }
            }
        }
        /* By this point, there are no cells with overlapping ships */
        return true;
    }
}
