using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class PieceManager : MonoBehaviour
{
    public GlobalState Instance;
    public GameObject mPiecePrefab;
    // mNoActiveInteraction is false when the user is moving/rotating a piece
    public bool mNoActiveInteraction;

    // Change mSizes to tweak the number/types of ships
    private int[] mSizes = { 2, 3, 3, 4, 5 };
    private List<BasePiece> mShips = null;


    public void Setup(Board board)
    {
        // Initialize dragging boolean
        mNoActiveInteraction = true;

        // Create ship pieces
        mShips = CreatePieces(board);

        // Place pieces
        PlacePieces(mShips, board);
    }

    private List<BasePiece> CreatePieces(Board board)
    {
        List<BasePiece> newPieces = new List<BasePiece>();
        int shipLength;

        for (int i = 0; i < mSizes.Length; i++)
        {
            // Create
            shipLength = mSizes[i];
            BasePiece newPiece = CreatePiece(shipLength);
            newPieces.Add(newPiece);

            // Setup
            newPiece.Setup(Instance.savedColorTheme.CellPieceHeadColor, this, shipLength);
        }

        return newPieces;
    }

    private BasePiece CreatePiece(int length) // (Type pieceType)
    {
        // Create new object
        GameObject newPieceObject = Instantiate(mPiecePrefab);
        newPieceObject.transform.SetParent(transform);

        // Set scale and position
        newPieceObject.transform.localScale = new Vector3(1, 1, 1);
        newPieceObject.transform.localRotation = Quaternion.identity;

        // Store new piece
        BasePiece newPiece = (BasePiece) newPieceObject.AddComponent(typeof(BasePiece));

        return newPiece;
    }

    private void PlacePieces(List<BasePiece> pieces, Board board)
    {
        // TODO increase this number
        for (int i = 0; i < pieces.Count; i++)
        {
            Debug.Log("Placed piece");
            pieces[i].Place(board.mAllCells[0, i * 2]);
        }
    }

    public string ExportShips()
    {
        string exportedShips = "";
        for (int i = 0; i < mShips.Count; i++)
        {
            exportedShips += mShips[i].ExportShip();
        }
        return exportedShips;
    }
}
