/**
 * \file NPuzzleSolver.hpp
 * \brief The solver for N-Puzzle problem.
 *
 * The solver would record the total amount of nodes expanded
 * and the max length of the search queue.
 *
 * It's actually a composition class for an NPuzzle::NPuzzleSearcher.
 */
#ifndef __NPUZZLE_SOLVER__
#define __NPUZZLE_SOLVER__

namespace NPuzzle
{
    /**
     * \brief The solver for N-Puzzle problem.
     */
    class NPuzzleSolver
    {
    private:
        // Statistic
        unsigned int totalNodesExpanded = 0;
        unsigned int maxQueueLength = 1; // The initial state is in queue.

        // Recorded visited states
        std::map<NPuzzleState, bool> visitedState;
        // Heuristic function
        NPuzzleCostFunction hFunc;

    public:
        //! In N-Puzzle problem, g(n) = depth.
        static int GFunc(NPuzzleNode node) { return node.getDepth(); }

    public:
        //! Get the total amount of nodes expanded.
        unsigned int getTotalNodesExpanded() const { return totalNodesExpanded; }
        //! Get the max length of the search queue.
        unsigned int getMaxQueueLength() const { return maxQueueLength; }

        //! Get the heuristic function (\c g(n)).
        NPuzzleCostFunction getHeuristicFunction() const { return hFunc; }
        /**
         * \brief Set the heuristic function (\c g(n)).
         *
         * It could be NPuzzle::GetUniformHeuristicCost, NPuzzle::GetMisplacedTileCount,
         * or NPuzzle::GetManhattanDistance.
         * \param hFunc The heuristic function to be set.
         */
        void setHeuristicFunction(NPuzzleCostFunction hFunc) { this->hFunc = hFunc; }

        /**
         * \brief Solve the N-Puzzle problem based on a initial state.
         * \param initialState The intital state.
         * \return The SearchResult indiciates whether there is a solution.
         */

        NPuzzleSearchResult solve(NPuzzleState initialState)
        {
            NPuzzleProblem problem(initialState);
            NPuzzleSearcher searcher(
                // The depth of initial state is 0.
                [](NPuzzleState state) -> NPuzzleNode { return NPuzzleNode(state, 0); },
                [](NPuzzleNode node) -> NPuzzleState { return node.getState(); },
                // The comparison function for the priority queue.
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
    
                        // Update associated fields
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
