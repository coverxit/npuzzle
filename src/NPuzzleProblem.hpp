/**
 * \file NPuzzleProblem.hpp
 * \brief The concrete Problem class NPuzzle::NPuzzleProblem for the N-Puzzle problem.
 */
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
    private:
        NPuzzleState finalState;

    public:
        /**
         * \param initialState The initial state.
         * \param finalState The final state. 
         */
        explicit NPuzzleProblem(NPuzzleState initialState, NPuzzleState finalState)
            : Problem(initialState), finalState(finalState) {}

        //! \copydoc Problem::goalTest
        bool goalTest(NPuzzleState state)
        {
            return finalState == state;
        }

        /**
         * \copydoc Problem::getOperators
         *
         * In N-Puzzle problem, operators are \c up, \c down, \c left and \c right.
         */
        std::vector<OperatorT> getOperators()
        {
            return std::vector<OperatorT>{ moveRight, moveDown, moveLeft, moveUp };
        }

    private:
        static OperationResultT moveLeft(NPuzzleState state)
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
        }

        static OperationResultT moveRight(NPuzzleState state)
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
        }

        static OperationResultT moveUp(NPuzzleState state)
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
        }

        static OperationResultT moveDown(NPuzzleState state)
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
        }
    };
}

#endif