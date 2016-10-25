/**
 * \file NPuzzleUtility.hpp
 * \brief Helper functions for simplifying operations.
 */
#ifndef __NPUZZLE_UTILITY__
#define __NPUZZLE_UTILITY__

namespace NPuzzle
{
    //! Convert the index of array into a position (row, col) of matrix.
    void indexToMatrix(int index, int& row, int& col)
    {
        row = index / matrixDemonstration;
        col = index % matrixDemonstration;
    }

    //! Convert a position (row, col) of matrix into the index of array.
    int matrixToIndex(int row, int col)
    {
        return row * matrixDemonstration + col;
    }

    //! Helper function in printing state beautifully.
    void printState(NPuzzleState state)
    {
        for (int row = 0; row < matrixDemonstration; row++)
        {
            for (int col = 0; col < matrixDemonstration; col++)
                cout << state[matrixToIndex(row, col)] << " ";
            cout << endl;
        }
    }
}

#endif