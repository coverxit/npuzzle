#ifndef __NPUZZLE_DEFINITION__
#define __NPUZZLE_DEFINITION__

namespace NPuzzle
{
    // The state was stored in an array.
    typedef std::array<int, demonstration + 1> NPuzzleState;

    // The node used in search queue
    class NPuzzleNode {
    private:
        NPuzzleState state;
        int depth;

    public:
        NPuzzleNode() : depth(-1) {} // -1 represents failure.
        NPuzzleNode(NPuzzleState state, int depth) : state(state), depth(depth) {};

        NPuzzleState getState() const { return state; }
        void setState(NPuzzleState state) { this->state = state; }

        int getDepth() const { return depth; }
        void setDepth(int depth) { this->depth = depth; }
    };

    typedef GeneralSearcher<NPuzzleState, NPuzzleNode, int> NPuzzleSearcher;
    typedef SearchResult<NPuzzleNode>                       NPuzzleSearchResult;
    typedef NPuzzleSearcher::QueueT                         NPuzzleQueue;
    typedef NPuzzleSearcher::ExpandResultT                  NPuzzleExpandResult;
    typedef NPuzzleSearcher::OperationResultT               NPuzzleOperationResult;
    typedef std::function<int(NPuzzleState)>                CostFunction; // g(n), h(n)
}

#endif