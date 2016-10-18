#ifndef __NPUZZLE_HEURISTIC_FUNCTION__
#define __NPUZZLE_HEURISTIC_FUNCTION__

namespace NPuzzle
{
    // Uniform Cost Search, h(n) = 0
    int getUniformHeuristicCost(NPuzzleNode node)
    {
        return 0;
    }

    // Calculate the count of misplaced tiles
    int getMisplacedTileCount(NPuzzleNode node)
    {
        auto state = node.getState();
        int count = 0;

        for (int i = 0; i < (int) state.size(); i++)
            if (state[i] > 0 && i + 1 != state[i]) // Not blank and misplaced
                count++;
        return count;
    };

    // Calculate Manhattan distance
    int getManhattanDistance(NPuzzleNode node)
    {
        auto state = node.getState();
        int distance = 0;

        for (int i = 0; i < (int) state.size(); i++)
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