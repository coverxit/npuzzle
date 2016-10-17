#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>
#include <functional>
#include <map>
using namespace std;

namespace NPuzzle
{
    // The n-puzzle demonstration, could be change to 15, 25, etc.
    constexpr int demonstration = 8;
    // The cost of every operation (up, down, left, right) on blank
    constexpr int moveCost = 1;
}

#include "GeneralSearch.hpp"
#include "NPuzzle.hpp"
using namespace NPuzzle;

class Node {
private:
    State state;
    int depth;

public:
    Node() : depth(-1) {}
    Node(State state, int depth) : state(state), depth(depth) {};

    State getState() const { return state; }
    void setState(State state) { this->state = state; }

    int getDepth() const { return depth; }
    void setDepth(int depth) { this->depth = depth; }
};

typedef GeneralSearcher<State, Node, int> NPuzzleSearcher;
typedef SearchResult<Node> NPuzzleSearchResult;
typedef NPuzzleSearcher::QueueT NPuzzleQueue;
typedef NPuzzleSearcher::ExpandResultT NPuzzleExpandResult;
typedef NPuzzleSearcher::OperationResultT NPuzzleOperationResult;
typedef std::function<int(State)> CostFunction; // g(n), h(n)

void printState(State state)
{
    for (int row = 0; row < matrixDemonstration; row++)
    {
        for (int col = 0; col < matrixDemonstration; col++)
            cout << state[matrixToIndex(row, col)] << " ";
        cout << endl;
    }
}

// Calculate the count of misplaced tiles
int getMisplacedTileCount(State state)
{
    int count = 0;
    for (unsigned int i = 0; i < state.size(); i++)
        if (state[i] > 0 && i + 1 != state[i]) // Not blank and misplaced
            count++;
    return count;
};

// Calculate Manhattan distance
int getManhattanDistance(State state) 
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

int main(int argc, char* argv[])
{
    int selection;

    // Statistic
    unsigned int totalNodesExpanded = 0;
    unsigned int maxQueueLength = 1; // The initial state is in queue.

    // State initialState{ 1, 5, 3, 4, 0, 6, 7, 2, 8 };
    State initialState{ 1, 2, 3, 4, 8, 0, 7, 6, 5 };
    std::map<State, bool> visitedState;

    NPuzzleProblem problem(initialState);
    NPuzzleSearcher searcher(
        // The depth of initial state is 0.
        [](State state) -> Node { return Node(state, 0); },
        [](Node node) -> State { return node.getState(); }
    );
    NPuzzleSearchResult result = NPuzzleSearchResult::Failure();
    CostFunction hFunc; // h(n) based on user's section

    cout << "Welcome to Renjie Wu's " << demonstration << "-puzzle solver." << endl;
    cout << "Type \"1\" to use a defualt puzzle, or \"2\" to enter your own puzzle." << endl;
    cin >> selection;
    if (selection == 2)
    {
        cout << "Enter your puzzle, use a zero to represent the blank" << endl;
        for (int col = 0; col < matrixDemonstration; col++)
        {
            cout << "Enter the row " << col + 1 << ", use space or tbs between numbers: ";
            for (int row = 0; row < matrixDemonstration; row++)
                cin >> initialState[matrixToIndex(row, col)];
        }
    }
    else
    {
        cout << "Using default puzzle:" << endl;
        printState(initialState);
    }
    cout << endl;

    cout << "Enter your choice of algorithm:" << endl;
    cout << "  1. Uniform Cost Search." << endl;
    cout << "  2. A* with the Misplaced Tile heuristic." << endl;
    cout << "  3. A* with the Manhattan distance heurisitic." << endl;
    cin >> selection;
    cout << endl;

    // h(n) based on selection
    switch (selection)
    {
    case 1: // Uniform Cost Search, h(n) = 0
        hFunc = [](State state) -> int { return 0; };
        break;

    case 2: // A* with Misplaced Tile
        hFunc = getMisplacedTileCount;
        break;

    case 3: // A* with Manhattan distance
        hFunc = getManhattanDistance;
        break;
    }

    cout << "Expanding state:" << endl;
    printState(initialState);
    visitedState[initialState] = true;
    cout << endl;

    result = searcher.generalSearch(&problem,
        [&](NPuzzleQueue queue, NPuzzleExpandResult expand) -> NPuzzleQueue {
            auto currentState = expand.getCurrentNode().getState();
            auto currentDepth = expand.getCurrentNode().getDepth();
            auto expandResult = expand.getExpandResult();

            // g(n) = depth + moveCost
            CostFunction gFunc = [&](State stete) { return currentDepth + moveCost; };

            // Calculate all g(n) & h(n)
            vector<int> g, h;
            for (auto res : expandResult)
            {
                g.push_back(gFunc(res.getState()));
                h.push_back(hFunc(res.getState()));
            }

            // Sort all expanded state by its cost g(n) + h(n)
            std::sort(expandResult.begin(), expandResult.end(), 
                [&](NPuzzleOperationResult& a, NPuzzleOperationResult& b) { 
                    auto stateA = a.getState(), stateB = b.getState();
                    return gFunc(stateA) + hFunc(stateA) < gFunc(stateB) + hFunc(stateB); 
                }
            );

            // Enqueue cheapest nodes
            State cheapestState = expandResult[0].getState();
            int cheapestCost = gFunc(cheapestState) + hFunc(cheapestState);
            for (auto res : expandResult)
            {
                auto state = res.getState();
                if (gFunc(state) + hFunc(state) > cheapestCost) 
                    break;

                // Has current state visited?
                if (visitedState[state])
                    continue;
                
                auto expandState = res.getState();
                cout << "The best state to expand with a g(n) = " << gFunc(expandState);
                cout << " and h(n) = " << hFunc(expandState) << " is..." << endl;
                printState(expandState);
                cout << "Expanding this node..." << endl;
                cout << endl;

                // Enqueue with depth + 1
                queue.push(Node(expandState, currentDepth + 1)); 
                
                totalNodesExpanded++;
                visitedState[state] = true;
            }

            // Is the size of queue larger?
            if (queue.size() > maxQueueLength)
                maxQueueLength = queue.size();

            return queue;
        }
    );

    if (!result.isSucceeded()) 
        cout << "No solution!" << endl; 
    else
    {
        cout << "Goal!!" << endl << endl;
        cout << "To solve this problem, the search alogrithm expanded a total of ";
        cout << totalNodesExpanded << " nodes." << endl;
        cout << "The maximum number of nodes in the queue at any one time was ";
        cout << maxQueueLength << "." << endl;
        cout << "The depth of the goal node was ";
        cout << result.getFinalNode().getDepth() << "." << endl;
    }
    return 0;
}