#ifndef __PUZZLE__
#define __PUZZLE__

namespace Puzzle
{
    constexpr int demonstration = 8;
    constexpr int moveCost = 1;

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

    class PuzzleProblem : public Problem<State, int>
    {
    public:
        PuzzleProblem(State initialState) : Problem(initialState) {}

        bool goalTest(State state)
        {
            for (unsigned int i = 0; i < state.size(); i++)
                if (i + 1 != state[i])
                    return false;

            return true;
        }

        std::vector<OperatorT> getOperators()
        {
            auto moveUp = [&](State state) -> OperationResultT {
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();

                indexToMatrix(blank, row, col);
                if (row - 1 >= 0)
                    std::swap(state[blank], state[matrixToIndex(row - 1, col)]);

                return OperationResultT(state, moveCost);
            };

            auto moveDown = [&](State state) -> OperationResultT {
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();

                indexToMatrix(blank, row, col);
                if (row + 1 < matrixDemonstration)
                    std::swap(state[blank], state[matrixToIndex(row + 1, col)]);

                return OperationResultT(state, moveCost);
            };

            auto moveLeft = [&](State state) -> OperationResultT {
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();

                indexToMatrix(blank, row, col);
                if (col - 1 >= 0)
                    std::swap(state[blank], state[matrixToIndex(row, col - 1)]);

                return OperationResultT(state, moveCost);
            };

            auto moveRight = [&](State state) -> OperationResultT {
                int row, col;
                int blank = std::find(state.begin(), state.end(), 0) - state.begin();

                indexToMatrix(blank, row, col);
                if (col + 1 < matrixDemonstration)
                    std::swap(state[blank], state[matrixToIndex(row, col + 1)]);

                return OperationResultT(state, moveCost);
            };

            return std::vector<OperatorT> {
                moveUp, moveDown, moveLeft, moveRight
            };
        }
    };
}

#endif