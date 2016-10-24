#ifndef __NPUZZLE_SOLVER__
#define __NPUZZLE_SOLVER__

namespace NPuzzle
{
    class NPuzzleSolver
    {
    private:
        // Statistic
        unsigned int totalNodesExpanded = 0;
        unsigned int maxQueueLength = 1; // The initial nextState is in queue.

        // Recored visited states
        std::map<NPuzzleState, bool> visitedState;
        // Heuristic function
        NPuzzleCostFunction hFunc;

    public:
        // g(n) = depth
        static int GFunc(NPuzzleNode node) { return node.getDepth(); }

    public:
        unsigned int getTotalNodesExpanded() const { return totalNodesExpanded; }
        unsigned int getMaxQueueLength() const { return maxQueueLength; }

        NPuzzleCostFunction getHeuristicFunction() const { return hFunc; }
        void setHeuristicFunction(NPuzzleCostFunction hFunc) { this->hFunc = hFunc; }

        NPuzzleSearchResult solve(NPuzzleState initialState)
        {
            NPuzzleProblem problem(initialState);
            NPuzzleSearcher searcher(
                // The depth of initial nextState is 0.
                [](NPuzzleState state) -> NPuzzleNode { return NPuzzleNode(state, 0); },
                [](NPuzzleNode node) -> NPuzzleState { return node.getState(); },
                [&](const NPuzzleNode& a, const NPuzzleNode& b) {
                    // The element with less f(n) has higher priority,
                    // which actually constructs a min-heap.
                    // (The STL heap is a max-heap default)
                    int costA = GFunc(a) + hFunc(a);
                    int costB = GFunc(b) + hFunc(b);

                    // If we got two nodes with the same priority,
                    // then we prefer the node with deeper depth.
                    if (costA == costB)
                        return GFunc(a) > GFunc(b);
                    else // Maintain a min-heap.
                        return costA > costB;
                }
            );

            visitedState[initialState] = true;
            auto result = searcher.generalSearch(&problem,
                // Queuing-Function
                [&](NPuzzleQueue queue, NPuzzleExpandResult expand) -> NPuzzleQueue 
                {
                    auto currentNode = expand.getCurrentNode();

                    // Print expanding information
                    if (currentNode.getState() != initialState)
                    {
                        cout << "The best state to expand with a g(n) = " << GFunc(currentNode);
                        cout << " and h(n) = " << hFunc(currentNode) << " is..." << endl;
                        printState(currentNode.getState());
                        cout << "Expanding this node..." << endl;
                        cout << endl;
                    }
                   
                    for (auto nextState : expand.getExpandedState())
                    {
                        // Has this expanded state visited?
                        if (visitedState[nextState])
                            continue;

                        // Enqueue a new node with expanded nextState and depth + 1
                        queue.push(NPuzzleNode(nextState, currentNode.getDepth() + 1));
    
                        // Update assoicated fields
                        totalNodesExpanded++;
                        visitedState[nextState] = true;

                        // Check if final nextState
                        if (problem.goalTest(nextState))
                            break;
                    }

                    // Is the size of current queue larger than previous?
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
