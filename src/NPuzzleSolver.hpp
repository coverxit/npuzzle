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
     *
     *  It's actually a composition class for an NPuzzle::NPuzzleSearcher.
     */
    class NPuzzleSolver
    {
    private:
        // Statistic
        unsigned long totalNodesExpanded = 0;
        unsigned long maxQueueLength = 1; // The initial state is in queue.

        // Record visited states, use hash(state) as the key.
        std::unordered_set<std::size_t> visitedState;
        // Heuristic function
        NPuzzleCostFunction hFunc;

        // Mapping from the children node to its parent node, used in tracing path
        std::unordered_map<NPuzzleNode, NPuzzleNode> expandMapping;
        // Record the final node for trace path
        NPuzzleNode finalNode;

    public:
        //! In N-Puzzle problem, g(n) = depth.
        static int GFunc(NPuzzleNode node) { return node.getDepth(); }

    public:
        //! Get the total amount of nodes expanded.
        unsigned long getTotalNodesExpanded() const { return totalNodesExpanded; }
        //! Get the max length of the search queue.
        unsigned long getMaxQueueLength() const { return maxQueueLength; }

        //! Get the heuristic function (\c h(n)).
        NPuzzleCostFunction getHeuristicFunction() const { return hFunc; }
        /**
         * \brief Set the heuristic function (\c h(n)).
         * \param hFunc The heuristic function to be set.
         *
         * \c hFunc could be NPuzzle::GetUniformHeuristicCost, NPuzzle::GetMisplacedTileCount,
         * or NPuzzle::GetManhattanDistance.
         */
        void setHeuristicFunction(NPuzzleCostFunction hFunc) { this->hFunc = hFunc; }

        //! Get the path to solution if exists. The path starts with the initial state.
        std::vector<NPuzzleNode> getSolutionPath()
        {
            std::vector<NPuzzleNode> path;

            // Last search is failed?
            if (finalNode.getDepth() == NPuzzleNode::FailureDepth)
                return std::vector<NPuzzleNode>();

            auto currentNode = finalNode;
            // The initialNode is mapped to a node with FailureDepth,
            // so it is used as the termination condition.
            while (expandMapping[currentNode].getDepth() != NPuzzleNode::FailureDepth)
            {
                path.push_back(expandMapping[currentNode]);
                currentNode = expandMapping[currentNode];
            }

            // The path now is in reverse order, so we reverse it back.
            std::reverse(path.begin(), path.end());
            return path;
        }

        /**
         * \brief Solve the N-Puzzle problem based on a initial state.
         * \param initialState The intital state.
         * \param finalState The goal state.
         * \return The SearchResult indiciates whether there is a solution.
         */
        NPuzzleSearchResult solve(NPuzzleState initialState, NPuzzleState finalState)
        {
            // Clear the information last search left.
            visitedState.clear();
            expandMapping.clear();
            totalNodesExpanded = 0;
            maxQueueLength = 1;

            // Constructs problem and searcher.
            NPuzzleProblem problem(initialState, finalState);
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
                    // then we prefer the node with shallower depth.
                    if (costA == costB)
                        return GFunc(a) > GFunc(b);
                    else // Maintain a min-heap.
                        return costA > costB;
                }
            );

            visitedState.insert(std::hash<NPuzzleState>()(initialState));
            auto result = searcher.generalSearch(&problem,
                // Queuing-Function
                [&](NPuzzleQueue& queue, NPuzzleExpandResult expand)
                {
                    auto currentNode = expand.getCurrentNode();
                   
                    for (auto nextState : expand.getExpandedState())
                    {
                        auto hashVal = std::hash<NPuzzleState>()(nextState);

                        // Has this expanded state visited?
                        if (visitedState.find(hashVal) != visitedState.end())
                            continue;

                        // Enqueue a new node with expanded nextState and depth + 1
                        auto newNode = NPuzzleNode(nextState, currentNode.getDepth() + 1);
                        queue.push(newNode);
                        // Mapping the new node to its parent node.
                        expandMapping[newNode] = currentNode;
    
                        // Update associated fields
                        totalNodesExpanded++;
                        visitedState.insert(hashVal);

                        // Check if nextState goal
                        if (problem.goalTest(nextState))
                            break;
                    }

                    // Is the size of current queue larger than previous?
                    if (queue.size() > maxQueueLength)
                        maxQueueLength = queue.size();
                }
            );

            // Set finalNode for later path tracing
            finalNode = result.getFinalNode();
            return result;
        }
    };
}

#endif
