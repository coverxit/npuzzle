#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include <vector>
#include <functional>
#include <map>
using namespace std;

#include "PriorityQueue.hpp"
#include "GeneralSearch.hpp"

#include "NPuzzleSetting.hpp"
#include "NPuzzleDefinition.hpp"
#include "NPuzzleUtility.hpp"
#include "NPuzzleProblem.hpp"
#include "NPuzzleHeuristicFunction.hpp"
#include "NPuzzleSolver.hpp"
using namespace NPuzzle;

int main(int argc, char* argv[])
{
    int selection;

    // NPuzzleState initialState{ 1, 5, 3, 4, 0, 6, 7, 2, 8 };
    NPuzzleState initialState{ 1, 2, 3, 4, 8, 0, 7, 6, 5 };
    NPuzzleSolver solver;

    cout << "Welcome to Renjie Wu's " << demonstration << "-puzzle solver." << endl;
    cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl;
    cin >> selection;
    if (selection == 2)
    {
        cout << "Enter your puzzle, use a zero to represent the blank" << endl;
        for (int row = 0; row < matrixDemonstration; row++)
        {
            cout << "Enter the row " << row + 1 << ", use space or tabs between numbers: ";
            for (int col = 0; col < matrixDemonstration; col++)
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
    cout << "  3. A* with the Manhattan distance heuristic." << endl;
    cin >> selection;
    cout << endl;

    // h(n) based on selection
    switch (selection)
    {
    case 1: // Uniform Cost Search
        solver.setHeuristicFunction(GetUniformHeuristicCost);
        break;

    case 2: // A* with Misplaced Tile
        solver.setHeuristicFunction(GetMisplacedTileCount);
        break;

    case 3: // A* with Manhattan distance
    default:
        solver.setHeuristicFunction(GetManhattanDistance);
        break;
    }

    cout << "Expanding state:" << endl;
    printState(initialState);
    cout << endl;

    auto result = solver.solve(initialState);
    if (!result.isSucceeded())
        cout << "No solution!" << endl;
    else
    {
        cout << "Goal!!" << endl << endl;
        cout << "To solve this problem, the search algorithm expanded a total of ";
        cout << solver.getTotalNodesExpanded() << " nodes." << endl;
        cout << "The maximum number of nodes in the queue at any one time was ";
        cout << solver.getMaxQueueLength() << "." << endl;
        cout << "The depth of the goal node was ";
        cout << result.getFinalNode().getDepth() << "." << endl;
    }

    return 0;
}