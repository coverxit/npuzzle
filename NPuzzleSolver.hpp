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
        // g(n) = depth
        static int GFunc(NPuzzleNode node) { return node.getDepth(); }

    public:
        unsigned int getTotalNodesExpanded() const { return totalNodesExpanded; }
        unsigned int getMaxQueueLength() const { return maxQueueLength; }

        CostFunction getHeuristicFunction() const { return hFunc; }
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
                   
                    for (auto state : expand.getExpandedState())
                    {
                        // Has this state visited?
                        if (visitedState[state])
                            continue;

                        // Enqueue a new node with expanded state and depth + 1
                        queue.push_back(NPuzzleNode(state, currentNode.getDepth() + 1));
                        // Keep it a heap
                        std::make_heap(queue.begin(), queue.end(), 
                            [&](const NPuzzleNode& a, const NPuzzleNode& b) {
                                // The element with less f(n) has higher priority,
                                // which actually constructs a min-heap.
                                // (The STL heap is a max-heap default)
                                return GFunc(a) + hFunc(a) > GFunc(b) + hFunc(b);
                            }
                        );

                        // Update assoicated fields
                        totalNodesExpanded++;
                        visitedState[state] = true;

                        // Check if final state
                        if (problem.goalTest(state))
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