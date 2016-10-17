#ifndef __NPUZZLE__
#define __NPUZZLE__

namespace NPuzzle
{
    const int matrixDemonstration = (int)sqrt(demonstration + 1);

    typedef std::array<int, demonstration + 1> State;

    void indexToMatrix(int pos, int& row, int& col)
    {
        row = pos / matrixDemonstration;
        col = pos % matrixDemonstration;
    }

    int matrixToIndex(int row, int col)
    {
        return row * matrixDemonstration + col;
    }

    class NPuzzleProblem : public Problem<State, int>
    {
    public:
        NPuzzleProblem(State initialState) : Problem(initialState) {}

        bool goalTest(State state)
        {
            for (unsigned int i = 0; i < demonstration; i++)
                if (i + 1 != state[i])
                    return false;

            return true;
        }

        std::vector<OperatorT> getOperators()
        {
            auto moveUp = [&](State state) -> OperationResultT {
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();
                bool canMove = false;

                indexToMatrix(blank, row, col);
                if (row - 1 >= 0) 
                {
                    canMove = true;
                    std::swap(state[blank], state[matrixToIndex(row - 1, col)]);
                }

                return OperationResultT(canMove, state, moveCost);
            };

            auto moveDown = [&](State state) -> OperationResultT {
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();
                bool canMove = false;

                indexToMatrix(blank, row, col);
                if (row + 1 < matrixDemonstration)
                {
                    canMove = true;
                    std::swap(state[blank], state[matrixToIndex(row + 1, col)]);
                }

                return OperationResultT(canMove, state, moveCost);
            };

            auto moveLeft = [&](State state) -> OperationResultT {
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();
                bool canMove = false;

                indexToMatrix(blank, row, col);
                if (col - 1 >= 0)
                {
                    canMove = true;
                    std::swap(state[blank], state[matrixToIndex(row, col - 1)]);
                }

                return OperationResultT(canMove, state, moveCost);
            };

            auto moveRight = [&](State state) -> OperationResultT {
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();
                bool canMove = false;

                indexToMatrix(blank, row, col);
                if (col + 1 < matrixDemonstration)
                {
                    canMove = true;
                    std::swap(state[blank], state[matrixToIndex(row, col + 1)]);
                }

                return OperationResultT(canMove, state, moveCost);
            };

            return std::vector<OperatorT> {
                moveUp, moveDown, moveLeft, moveRight
            };
        }
    };
}

#endif