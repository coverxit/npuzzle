#ifndef __NPUZZLE_PROBLEM__
#define __NPUZZLE_PROBLEM__

namespace NPuzzle
{
    // The cost type is apparently int. 
    class NPuzzleProblem : public Problem<NPuzzleState, int>
    {
    public:
        NPuzzleProblem(NPuzzleState initialState) : Problem(initialState) {}

        // Test if final state achieved.
        bool goalTest(NPuzzleState state)
        {
            // The last number in state is 0, thus only check
            // the first demonstration elements.
            for (unsigned int i = 0; i < demonstration; i++)
                if (i + 1 != state[i])
                    return false;

            return true;
        }

        // Operators (up, down, left, right) in N-Puzzle problem.
        std::vector<OperatorT> getOperators()
        {
            // Move up
            auto moveUp = [&](NPuzzleState state) -> OperationResultT {
                bool canMove = false;
                
                // Find blank position.
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();
                indexToMatrix(blank, row, col);

                if (row - 1 >= 0) // check boundary
                {
                    canMove = true;
                    std::swap(state[blank], state[matrixToIndex(row - 1, col)]);
                }

                return canMove ? OperationResultT::Success(state, moveCost) 
                               : OperationResultT::Failure();
            };

            // Move down
            auto moveDown = [&](NPuzzleState state) -> OperationResultT {
                bool canMove = false;

                // Find blank position.
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();
                indexToMatrix(blank, row, col);

                if (row + 1 < matrixDemonstration) // check boundary
                {
                    canMove = true;
                    std::swap(state[blank], state[matrixToIndex(row + 1, col)]);
                }

                return canMove ? OperationResultT::Success(state, moveCost)
                               : OperationResultT::Failure();
            };

            // Move left
            auto moveLeft = [&](NPuzzleState state) -> OperationResultT {
                bool canMove = false;

                // Find blank position.
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();
                indexToMatrix(blank, row, col);

                if (col - 1 >= 0) // check boundary
                {
                    canMove = true;
                    std::swap(state[blank], state[matrixToIndex(row, col - 1)]);
                }

                return canMove ? OperationResultT::Success(state, moveCost)
                               : OperationResultT::Failure();
            };

            // Move right
            auto moveRight = [&](NPuzzleState state) -> OperationResultT {
                bool canMove = false;

                // Find blank position.
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();
                indexToMatrix(blank, row, col);

                if (col + 1 < matrixDemonstration) // check boundary
                {
                    canMove = true;
                    std::swap(state[blank], state[matrixToIndex(row, col + 1)]);
                }

                return canMove ? OperationResultT::Success(state, moveCost)
                               : OperationResultT::Failure();
            };

            return std::vector<OperatorT> {
                moveUp, moveDown, moveLeft, moveRight
            };
        }
    };
}

#endif