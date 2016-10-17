#ifndef __GENERAL_SERACH__
#define __GENERAL_SEARCH__

template <class StateT, typename ExpandCostT>
class OperationResult
{
private:
    bool succeeded;
    StateT state;
    ExpandCostT cost;

public:
    OperationResult(bool succeeded, StateT state, ExpandCostT cost) 
        : succeeded(succeeded), state(state), cost(cost) {}

    bool isSucceeded() const { return succeeded; }
    StateT getState() const { return state; }
    ExpandCostT getCost() const { return cost; }
};

template <class StateT, typename ExpandCostT>
class Problem
{
protected:
    typedef OperationResult<StateT, ExpandCostT> OperationResultT;
    typedef std::function<OperationResultT(StateT)> OperatorT;

private:
    StateT initialState;

public:
    Problem(StateT initialState) : initialState(initialState) {}

    StateT getInitialState() const { return initialState; }
    void setInitialState(StateT state) { initialState = state; }

    virtual bool goalTest(StateT state) = 0;
    virtual std::vector<OperatorT> getOperators() = 0;
};

template <class NodeT>
class SearchResult
{
    typedef SearchResult<NodeT> SearchResultT;

private:
    bool succeeded;
    NodeT finalNode;

private:
    SearchResult() : succeeded(false) {}
    SearchResult(NodeT finalNode) : finalNode(finalNode) {}

public:
    bool isSucceeded() const { return succeeded; }
    NodeT getFinalNode() const { return finalNode; }

public:
    static SearchResultT Failure() { return SearchResultT(); }
    static SearchResultT Success(NodeT finalNode) { return SearchResultT(finalNode); }
};

template <class StateT, class NodeT, typename ExpandCostT>
class ExpandResult
{
    typedef OperationResult<StateT, ExpandCostT> OperationResultT;
    typedef std::function<OperationResultT(StateT)> OperatorT;
    typedef std::vector<OperationResultT> OperationResultVectorT;

private:
    OperationResultVectorT opResults;
    NodeT currentNode;

public:
    ExpandResult(NodeT expandedNode, OperationResultVectorT opResults) :
        currentNode(expandedNode), opResults(opResults) {}

    NodeT getCurrentNode() const { return currentNode; }
    OperationResultVectorT getExpandResult() const { return opResults; }
};

template <class StateT, class NodeT, typename ExpandCostT>
class GeneralSearcher
{
public:
    typedef std::queue<NodeT> QueueT;
    typedef OperationResult<StateT, ExpandCostT> OperationResultT;
    typedef ExpandResult<StateT, NodeT, ExpandCostT> ExpandResultT;
    typedef std::function<QueueT(QueueT, ExpandResultT)> QueuingFunctionT;
    typedef SearchResult<NodeT> SearchResultT;

private:
    typedef Problem<StateT, ExpandCostT> ProblemT;
    typedef std::function<OperationResultT(StateT)> OperatorT;
    typedef std::vector<OperationResultT> OperationResultVectorT;
    typedef std::function<NodeT(StateT)> QueueNodeMakerT;
    typedef std::function<StateT(NodeT)> NodeToStateT;

private:
    QueueNodeMakerT makeNode;
    NodeToStateT nodeToState;

private:
    ExpandResultT expand(NodeT node, std::vector<OperatorT> operators)
    {
        OperationResultVectorT ret;
        for (auto move : operators)
        {
            auto res = move(nodeToState(node));
            if (res.isSucceeded())
                ret.push_back(res);
        }
          
        return ExpandResultT(node, ret);
    }

public:
    GeneralSearcher(QueueNodeMakerT makeNode, NodeToStateT nodeToState) 
        : makeNode(makeNode), nodeToState(nodeToState) {}

    SearchResultT generalSearch(ProblemT* problem, QueuingFunctionT queueingFunction)
    {
        QueueT nodes;
        nodes.push(makeNode(problem->getInitialState()));

        while (true)
        {
            if (nodes.empty()) 
                return SearchResultT::Failure();

            auto node = nodes.front();
            nodes.pop();

            if (problem->goalTest(nodeToState(node)))
                return SearchResultT::Success(node);

            nodes = queueingFunction(nodes, expand(node, problem->getOperators()));
        }
    }
};

#endif