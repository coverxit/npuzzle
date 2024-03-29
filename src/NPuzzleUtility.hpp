/**
 * \file NPuzzleUtility.hpp
 * \brief Helper functions for simplifying operations.
 */
#ifndef __NPUZZLE_UTILITY__
#define __NPUZZLE_UTILITY__

namespace NPuzzle
{
    //! Convert the index of array into a position (row, col) of matrix.
    void indexToMatrix(long index, int& row, int& col)
    {
        row = static_cast<int>(index / matrixDemonstration);
        col = static_cast<int>(index % matrixDemonstration);
    }

    //! Convert a position (row, col) of matrix into the index of array.
    long matrixToIndex(int row, int col)
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

    //! Helper function for checking solvability.
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
            auto blank = std::find(state.begin(), state.end(), 0) - state.begin();
            indexToMatrix(blank, row, col);

            if ((matrixDemonstration - row) % 2 == 0)   // even blank
                return countInversions(state) % 2 != 0; // odd inversions
            else                                        // odd blank
                return countInversions(state) % 2 == 0; // even inversions
        }
    }
}

namespace std
{
    //! Specalization template for hashing a NPuzzle::NPuzzleState.
    template<>
    struct hash<NPuzzle::NPuzzleState>
    {
        std::size_t operator()(const NPuzzle::NPuzzleState& state) const
        {
            std::size_t ret = 17;
            for (auto& e : state)
                ret = ret * 31 + std::hash<int>()(e);
            return ret;
        }
    };

    //! Specialzation template for hashing a NPuzzle::NPuzzleNode
    template<>
    struct hash<NPuzzle::NPuzzleNode>
    {
        std::size_t operator()(const NPuzzle::NPuzzleNode& node) const
        {
            std::size_t ret = 17;
            ret = ret * 31 + hash<int>()(node.getDepth());
            ret = ret * 31 + hash<NPuzzle::NPuzzleState>()(node.getState());
            return ret;
        }
    };

    //! Specialzation template for comparing NPuzzle::NPuzzleNode.
    template<>
    struct equal_to<NPuzzle::NPuzzleNode>
    {
        bool operator()(const NPuzzle::NPuzzleNode& a, const NPuzzle::NPuzzleNode& b) const
        {
            auto hasher = hash<NPuzzle::NPuzzleState>();
            return a.getDepth() == b.getDepth() &&
                   hasher(a.getState()) == hasher(b.getState());
        }
    };
}

#endif