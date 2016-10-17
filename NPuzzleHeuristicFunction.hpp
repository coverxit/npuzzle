#ifndef __NPUZZLE_HEURISTIC_FUNCTION__
#define __NPUZZLE_HEURISTIC_FUNCTION__

namespace NPuzzle
{
    // The h(n) for Uniform Cost Search shall be
    //   [](NPuzzleState state) -> int { return 0; }
    // which is placed in main.cpp

    // Calculate the count of misplaced tiles
    int getMisplacedTileCount(NPuzzleState state)
    {
        int count = 0;
        for (unsigned int i = 0; i < state.size(); i++)
            if (state[i] > 0 && i + 1 != state[i]) // Not blank and misplaced
                count++;
        return count;
    };

    // Calculate Manhattan distance
    int getManhattanDistance(NPuzzleState state)
    {
        int distance = 0;
        for (unsigned int i = 0; i < state.size(); i++)
        {
            if (state[i] > 0 && i + 1 != state[i]) // Not blank and misplaced
            {
                // Get current position and find target position
                int row, col, targetRow, targetCol;
                indexToMatrix(i, row, col);
                indexToMatrix(state[i] - 1, targetRow, targetCol);

                // This distance is steps needed by moving to target position.
                distance += abs(targetRow - row) + abs(targetCol - col);
            }
        }
        return distance;
    };
}

#endif