#ifndef __NPUZZLE_DEFINITION__
#define __NPUZZLE_DEFINITION__

//! N-Puzzle problem namespace
namespace NPuzzle
{
    //! The state was sotored in a \c std::array.
    typedef std::array<int, demonstration + 1> NPuzzleState;

    //! The node used in search queue.
    class NPuzzleNode {
    private:
        NPuzzleState state;
        int depth;

    public:
        NPuzzleNode() : depth(-1) {} // -1 represents failure.

        /**
         * \param state The state to be stored in node.
         * \param depth The depth to be stored in node.
         * \return A queue node with specific state and depth.
         */
        NPuzzleNode(NPuzzleState state, int depth) : state(state), depth(depth) {};

        //! Get the state stored in node.
        NPuzzleState getState() const { return state; }
        //! Set the state stored in node.
        void setState(NPuzzleState state) { this->state = state; }

        //! Get the expanded depth of this node.
        int getDepth() const { return depth; }
        //! Set the expanded depth of this node.
        void setDepth(int depth) { this->depth = depth; }
    };

    /**
     * \brief The N-Puzzle searcher, with NPuzzle::NPuzzleState as \c StateT,
     * NPuzzle::NPuzzleNode as \c NodeT and \c int as \c ExpandCostT.
     * \see GeneralSearcher
     */
    typedef GeneralSearcher<NPuzzleState, NPuzzleNode, int> NPuzzleSearcher;
    typedef NPuzzleSearcher::SearchResultT                  NPuzzleSearchResult;
    typedef NPuzzleSearcher::QueueT                         NPuzzleQueue;
    typedef NPuzzleSearcher::ExpandResultT                  NPuzzleExpandResult;
    typedef NPuzzleSearcher::OperationResultT               NPuzzleOperationResult;
    /**
     * \brief The cost function type for \c g(n) and \c h(n).
     *
     * It should be the following form:
     * \code
     * int costFunction(NPuzzleNode node)
     * {
     *     ...
     * }
     * \endcode
     */
    typedef std::function<int(NPuzzleNode)>                 NPuzzleCostFunction; // g(n), h(n)
}

#endif