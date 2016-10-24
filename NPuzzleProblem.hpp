#ifndef __NPUZZLE_PROBLEM__
#define __NPUZZLE_PROBLEM__

namespace NPuzzle
{
    /**
     * \brief The class describes the N-Puzzle problem.
     *
     * The \c StateT is NPuzzle::NPuzzleState and the \c ExpandCostT is \c int.
     *
     * \see Problem
     */
    class NPuzzleProblem : public Problem<NPuzzleState, int>
    {
    public:
        //! Refer to \ref Problem::Problem.
        explicit NPuzzleProblem(NPuzzleState initialState) : Problem(initialState) {}

        //! \copydoc Problem::goalTest
        bool goalTest(NPuzzleState state)
        {
            // The last number in state is 0, thus only check
            // the first demonstration elements.
            for (int i = 0; i < demonstration; i++)
                if (i + 1 != state[i])
                    return false;

            return true;
        }

        /**
         * \copydoc Problem::getOperators
         *
         * In N-Puzzle problem, operators are \c up, \c down, \c left and \c right.
         */
        std::vector<OperatorT> getOperators()
        {
            // Move up
            auto moveUp = [&](NPuzzleState state) -> OperationResultT 
            {
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
            auto moveDown = [&](NPuzzleState state) -> OperationResultT 
            {
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
            auto moveLeft = [&](NPuzzleState state) -> OperationResultT 
            {
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
            auto moveRight = [&](NPuzzleState state) -> OperationResultT
            {
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

            return std::vector<OperatorT> { moveUp, moveDown, moveLeft, moveRight };
        }
    };
}

#endif