#ifndef __NPUZZLE_SOLVER__
#define __NPUZZLE_SOLVER__

namespace NPuzzle
{
    class NPuzzleSolver
    {
    private:
        // Statistic
        unsigned int totalNodesExpanded = 0;
        unsigned int maxQueueLength = 1; // The initial state is in queue.

        // Recored visited states
        std::map<NPuzzleState, bool> visitedState;
        // Heuristic function
        CostFunction hFunc;

    public:
        unsigned int getTotalNodesExpanded() const { return totalNodesExpanded; }
        unsigned int getMaxQueueLength() const { return maxQueueLength; }

        void setHeuristicFunction(CostFunction hFunc) { this->hFunc = hFunc; }

        NPuzzleSearchResult solve(NPuzzleState initialState)
        {
            NPuzzleProblem problem(initialState);
            NPuzzleSearcher searcher(
                // The depth of initial state is 0.
                [](NPuzzleState state) -> NPuzzleNode { return NPuzzleNode(state, 0); },
                [](NPuzzleNode node) -> NPuzzleState { return node.getState(); }
            );

            visitedState[initialState] = true;
            auto result = searcher.generalSearch(&problem,
                // Queuing-Function
                [&](NPuzzleQueue queue, NPuzzleExpandResult expand) -> NPuzzleQueue 
                {
                    auto currentState = expand.getCurrentNode().getState();
                    auto currentDepth = expand.getCurrentNode().getDepth();
                    auto expandResult = expand.getExpandResult();

                    // g(n) = depth + moveCost
                    CostFunction gFunc = [&](NPuzzleState stete) { return currentDepth + moveCost; };

                    // Calculate all g(n) & h(n)
                    vector<int> g, h;
                    for (auto res : expandResult)
                    {
                        g.push_back(gFunc(res.getState()));
                        h.push_back(hFunc(res.getState()));
                    }

                    // Sort all expanded state by its cost g(n) + h(n)
                    std::sort(expandResult.begin(), expandResult.end(),
                        [&](NPuzzleOperationResult& a, NPuzzleOperationResult& b) -> int
                        {
                            auto stateA = a.getState(), stateB = b.getState();
                            return gFunc(stateA) + hFunc(stateA) < gFunc(stateB) + hFunc(stateB);
                        }
                    );

                    // Enqueue cheapest nodes
                    NPuzzleState cheapestState = expandResult[0].getState();
                    int cheapestCost = gFunc(cheapestState) + hFunc(cheapestState);
                    for (auto res : expandResult)
                    {
                        auto state = res.getState();
                        if (gFunc(state) + hFunc(state) > cheapestCost)
                            break;

                        // Has current state visited?
                        if (visitedState[state])
                            continue;

                        // Enqueue with depth + 1
                        queue.push(NPuzzleNode(state, currentDepth + 1));

                        totalNodesExpanded++;
                        visitedState[state] = true;

                        // Check if final state
                        if (problem.goalTest(state))
                            break;

                        // Print expanding information
                        cout << "The best state to expand with a g(n) = " << gFunc(state);
                        cout << " and h(n) = " << hFunc(state) << " is..." << endl;
                        printState(state);
                        cout << "Expanding this node..." << endl;
                        cout << endl;
                    }

                    // Is the size of queue larger?
                    if (queue.size() > maxQueueLength)
                        maxQueueLength = queue.size();

                    return queue;
                }
            );
            
            return result;
        }
    };
}

#endif