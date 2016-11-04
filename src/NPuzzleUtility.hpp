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

    //! Count inversions in a given state.
    int countInversions(NPuzzleState state)
    {
        int count = 0;
        for (int i = 0; i < demonstration; i++)
            for (int j = i + 1; j <= demonstration; j++)
                // 0 is blank tile.
                if (state[i] && state[j] && state[i] > state[j])
                    count++;
        return count;
    }

    //! Helpr function for checking solvability.
    bool isSolvable(NPuzzleState state)
    {
        if (matrixDemonstration % 2)
        {
            // If the grid width is odd, the number of inversions in solvable
            // state should be even.
            return countInversions(state) % 2 == 0;
        }
        else
        {
            // For a even grid, if the blank on an even row counting from the bottom,
            // then the number of inversions in solvable state should be odd.
            // Otherwise, if the blank on an odd row, the number of inversions should be even.
            int row, col;
            int blank = std::find(state.begin(), state.end(), 0) - state.begin();
            indexToMatrix(blank, row, col);

            if ((matrixDemonstration - row) % 2 == 0)   // even blank
                return countInversions(state) % 2 != 0; // odd inversions
            else                                        // odd blank
                return countInversions(state) % 2 == 0; // even inversions
        }
    }

    //! Helper function for output time friendly.
    std::string friendlyTime(std::chrono::duration<double> diff)
    {
        using namespace std::chrono;

        std::stringstream stream;
        if (duration_cast<microseconds>(diff).count() < 1000)
            stream << duration_cast<microseconds>(diff).count() << " μs";
        else if (duration_cast<milliseconds>(diff).count() < 1000)
            stream << duration_cast<milliseconds>(diff).count() << " ms";
        else
            stream << duration_cast<seconds>(diff).count() << " s";
        return stream.str();
    }
}

#endif