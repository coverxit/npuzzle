#ifndef __GENERAL_SERACH__
#define __GENERAL_SEARCH__

// The result of operations in a general problem with
// state type StateT and operation cost type ExpandCostT.
// OperationResult could only be created by its static creators.
// Take 8-Puzzle as example, the StateT should be an array,
// and the ExpandCostT should be int.
// The class should represent the result of moving blank up, down,
// left and right.
template <class StateT, typename ExpandCostT>
class OperationResult
{
    typedef OperationResult<StateT, ExpandCostT> OperationResultT;

private:
    bool succeeded;
    StateT state;
    ExpandCostT cost;

private:
    OperationResult() : succeeded(false) {}
    OperationResult(StateT state, ExpandCostT cost)
        : succeeded(true), state(state), cost(cost) {}

public:
    bool isSucceeded() const { return succeeded; }
    StateT getState() const { return state; }
    ExpandCostT getCost() const { return cost; }

public:
    static OperationResultT Failure() { return OperationResult(); }
    static OperationResultT Success(StateT state, ExpandCostT cost)
    {
        return OperationResult(state, cost);
    }
};

// General problem with state type StateT and expand cost type ExpandCostT.
// For 8-puzzle, the StateT is an array, the ExpandCostT is int.
template <class StateT, typename ExpandCostT>
class Problem
{
protected:
    typedef OperationResult<StateT, ExpandCostT>    OperationResultT;
    typedef std::function<OperationResultT(StateT)> OperatorT;

private:
    StateT initialState;

public:
    Problem(StateT initialState) : initialState(initialState) {}

    StateT getInitialState() const { return initialState; }
    void setInitialState(StateT state) { initialState = state; }

public:
    virtual bool goalTest(StateT state) = 0;
    virtual std::vector<OperatorT> getOperators() = 0;
};

// The result of general search, storing the final node with queue node type NodeT.
// SearchResult could only be created by its static creators.
template <class NodeT>
class SearchResult
{
    typedef SearchResult<NodeT> SearchResultT;

private:
    bool succeeded;
    NodeT finalNode;

private:
    SearchResult() : succeeded(false) {}
    SearchResult(NodeT finalNode) : succeeded(true), finalNode(finalNode) {}

public:
    bool isSucceeded() const { return succeeded; }
    NodeT getFinalNode() const { return finalNode; }

public:
    static SearchResultT Failure() { return SearchResultT(); }
    static SearchResultT Success(NodeT finalNode) { return SearchResultT(finalNode); }
};

// The result of a general expanded node with state type StateT 
// and queue node type NodeT.
// For 8-puzzle, the StateT is an array.
template <class StateT, class NodeT>
class ExpandResult
{
    typedef std::vector<StateT> ExpandedStateVectorT;

private:
    NodeT currentNode;
    ExpandedStateVectorT expandedState;

public:
    ExpandResult(NodeT expandedNode, ExpandedStateVectorT expandedState) :
        currentNode(expandedNode), expandedState(expandedState) {}

    NodeT getCurrentNode() const { return currentNode; }
    ExpandedStateVectorT getExpandedState() const { return expandedState; }
};

// The general searcher with state type StateT, queue node type NodeT
// and expand cost type ExpandCostT.
// For 8-puzzle, the StateT is an array, the ExpandCostT is int.
template <class StateT, class NodeT, typename ExpandCostT>
class GeneralSearcher
{
public:
    // Use vector as the underlying container for STL heap operations.
    typedef std::vector<NodeT>                              QueueT;
    typedef std::function<bool(const NodeT&, const NodeT&)> QueueComparatorT;
    typedef OperationResult<StateT, ExpandCostT>            OperationResultT;
    typedef ExpandResult<StateT, NodeT>                     ExpandResultT;
    typedef std::function<QueueT(QueueT, ExpandResultT)>    QueuingFunctionT;
    typedef SearchResult<NodeT>                             SearchResultT;

private:
    typedef Problem<StateT, ExpandCostT>                    ProblemT;
    typedef std::function<OperationResultT(StateT)>         OperatorT; 
    typedef std::vector<StateT>                             ExpandedStateVectorT;
    typedef std::function<NodeT(StateT)>                    NodeMakerT;
    typedef std::function<StateT(NodeT)>                    ToStateT;

private:
    // Converters between StateT and NodeT.
    NodeMakerT makeNode;
    ToStateT toState;
    // The comparator function used on STL heap operations.
    QueueComparatorT comparator;

private:
    ExpandResultT expand(NodeT node, std::vector<OperatorT> operators)
    {
        ExpandedStateVectorT ret;
        for (auto move : operators)
        {
            auto res = move(toState(node));
            // Only expand nodes on which operations succeeded.
            if (res.isSucceeded())
                ret.push_back(res.getState());
        }
        return ExpandResultT(node, ret);
    }

public:
    GeneralSearcher(NodeMakerT makeNode, ToStateT toState, QueueComparatorT comparator)
        : makeNode(makeNode), toState(toState), comparator(comparator) {}

    // function general-search(problem, QUEUEING-FUNCTION)
    SearchResultT generalSearch(ProblemT* problem, QueuingFunctionT queueingFunction)
    {
        // nodes = MAKE-QUEUE(MAKE-NODE(problem, INITIAL-STATE)
        QueueT nodes{ makeNode(problem->getInitialState()) }; // Enqueue initial state

        while (true)
        {
            // if EMPTY(nodes) then return "failure"
            if (nodes.empty()) 
                return SearchResultT::Failure();

            // node = REMOVE-FRONT(nodes)
            auto node = nodes.front();
            // Adjust heap and remove the highest-priority element
            std::pop_heap(nodes.begin(), nodes.end(), comparator);
            nodes.pop_back();

            // if problem.GOTL-TEST(node.STATE) succeeds then return node
            if (problem->goalTest(toState(node)))
                return SearchResultT::Success(node);

            // nodes = QUEUING-FUNCTION(nodes, EXPAND(node, problem.OPERATORS))
            nodes = queueingFunction(nodes, expand(node, problem->getOperators()));
        }
    }
};

#endif