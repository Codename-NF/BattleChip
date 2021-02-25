using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class PieceManager : MonoBehaviour
{
    public GameObject mPiecePrefab;

    private List<BasePiece> mShips = null;

    private string[] mShipTypes = new string[2]
    {
        //"2", "2", "3", "4", "5"
        "2", "2"
    };

    private Dictionary<string, Type> mPieceLibrary = new Dictionary<string, Type>()
    {
        {"2", typeof(SmallShip) },
        /*
        {"3", typeof(MediumShip) },
        {"4", typeof(LargeShip) },
        {"5", typeof(GiantShip) },
        */
    };

    public void Setup(Board board)
    {
        // Create ship pieces
        mShips = CreatePieces(board);

        // Place pieces
        PlacePieces(mShips, board);
    }

    private List<BasePiece> CreatePieces(Board board)
    {
        List<BasePiece> newPieces = new List<BasePiece>();

        for (int i = 0; i < mShipTypes.Length; i++)
        {
            // Get the type
            string key = mShipTypes[i];
            Type pieceType = mPieceLibrary[key];

            // Create
            BasePiece newPiece = CreatePiece(pieceType);
            newPieces.Add(newPiece);

            // Setup
            newPiece.Setup(new Color32(120, 73, 80, 255), this);
        }

        return newPieces;
    }

    private BasePiece CreatePiece(Type pieceType)
    {
        // Create new object
        GameObject newPieceObject = Instantiate(mPiecePrefab);
        newPieceObject.transform.SetParent(transform);

        // Set scale and position
        newPieceObject.transform.localScale = new Vector3(1, 1, 1);
        newPieceObject.transform.localRotation = Quaternion.identity;

        // Store new piece
        BasePiece newPiece = (BasePiece)newPieceObject.AddComponent(pieceType);

        return newPiece;
    }

    private void PlacePieces(List<BasePiece> pieces, Board board)
    {
        // TODO increase this number
        for (int i = 0; i < 2; i++)
        {
            pieces[i].Place(board.mAllCells[1, i + 1]);
        }
    }
    /*
    private void SetInteractive(List<BasePiece> allPieces, bool value)
    {
        foreach (BasePiece piece in allPieces)
            piece.enabled = value;
    } */
}
