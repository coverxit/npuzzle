/**
 * \file GeneralSearch.hpp
 * \brief The classes to define a (abstract) general searcher.
 */
#ifndef __GENERAL_SEARCH__
#define __GENERAL_SEARCH__

/**
 * \brief The result of operations in a general problem.
 * \tparam StateT The state type used in a Problem.
 * \tparam ExpandCostT The expanding cost type used in a Problem.
 *
 * OperationResult could only be created by its two static creators
 * OperationResult::Failure and OperationResult::Success.
 *
 * Take 8-Puzzle as example, the \c StateT should be an \c std::array,
 * and the \c ExpandCostT should be \c int.
 *
 * The class should represent the result of moving the blank tile
 * up, down, left and right.
 */
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
    /**
     * \brief Whether the operation succeded.
     * \return True if succeded, false if not.
     */
    bool isSucceeded() const { return succeeded; }
    //! The expanded state if the operation succeded.
    StateT getState() const { return state; }
    //! The expanding cost if the operation succeded.
    ExpandCostT getCost() const { return cost; }

public:
    //! Create a failed result, that is no expanded states by appling this operation.
    static OperationResultT Failure() { return OperationResult(); }
    /**
     * \brief Create a successful result, that is existing expanded states.
     * \param state The expanded state by this operation.
     * \param cost The cost of this operation.
     * \return A successful OperationResult with expanded state and its cost.
     */
    static OperationResultT Success(StateT state, ExpandCostT cost)
    {
        return OperationResult(state, cost);
    }
};

/**
 * \brief The general (abstract) problem class.
 * \tparam StateT The state type.
 * \tparam ExpandCostT The expanding cost type of operations.
 *
 * For 8-puzzle, the \c StateT is \c std::array and \c ExpandCostT is \c int.
 */
template <class StateT, typename ExpandCostT>
class Problem
{
public:
    typedef OperationResult<StateT, ExpandCostT>    OperationResultT;
    /**
     * \brief The operation function type.
     *
     * It should be the following form:
     * \code
     * OperationResult<StateT, ExpandCostT> operation(StateT state)
     * {
     *     ...
     * }
     * \endcode
     */
    typedef std::function<OperationResultT(StateT)> OperatorT;

protected:
    StateT initialState;

public:
    /**
     * \param initialState The initial state.
     */
    explicit Problem(StateT initialState) : initialState(initialState) {}

    //! Get the initial state.
    StateT getInitialState() const { return initialState; }
    //! Set the intial state.
    void setInitialState(StateT state) { initialState = state; }

public:
    /**
     * \brief Test if a state is final state.
     * \param state The state to be tested.
     * \return True if state is final state, false if not.
     */
    virtual bool goalTest(StateT state) = 0;

    /**
     * \brief Get all possible operations on a specific problem.
     * \return Vector of OperatorT on a problem.
     */
    virtual std::vector<OperatorT> getOperators() = 0;
};

/**
 * \brief The result of a general search.
 * \tparam NodeT The node type used in a PriorityQueue.
 *
 * It stores the final node with queue node type \c NodeT.
 *
 * SearchResult could only be created by its static creators
 * SearchResult::Failure and SearchResult::Success.
 */
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
    /**
     * \brief Whether there is a solution.
     * \return True if yes, false if not.
     */
    bool isSucceeded() const { return succeeded; }
    //! The final node if there exists a solution.
    NodeT getFinalNode() const { return finalNode; }

public:
    //! Create a failed result, that is no solution.
    static SearchResultT Failure() { return SearchResultT(); }
    /**
     * \brief Create a successful result, that is existing a solution.
     * \param finalNode The final Node.
     * \return A successful SearchResult with expanded state and its cost.
     */
    static SearchResultT Success(NodeT finalNode) { return SearchResultT(finalNode); }
};

/**
 * \brief The result of expanding a node in the queue.
 * \tparam StateT The state type used in a Problem.
 * \tparam NodeT The node type used in a GeneralSearcher.
 * \tparam ExpandCostT The expanding cost type used in a Problem.
 *
 * For 8-puzzle, the \c StateT is \c std::array.
 */
template <class StateT, class NodeT, class ExpandCostT>
class ExpandResult
{
public:
    typedef std::pair<StateT, ExpandCostT> ResultPairT;
    typedef std::vector<ResultPairT>       ResultVectorT;

private:
    NodeT currentNode;
    ResultVectorT result;

public:
    /**
     * \brief Create a expanding result.
     * \param expandedNode The node expanded.
     * \param result The states expanded and their cost from the node.
     */
    ExpandResult(NodeT expandedNode, ResultVectorT result) :
        currentNode(expandedNode), result(result) {}

    //! Get the expanded node.
    NodeT getCurrentNode() const { return currentNode; }
    //! Get states expanded and their cost from the node.
    ResultVectorT getResult() const { return result; }
};

/**
 * \brief The general (abstract) searcher.
 * \tparam StateT The state type used in a Problem.
 * \tparam NodeT The node type used in a PriorityQueue.
 * \tparam ExpandCostT The expanding cost type used in a Problem.
 *
 * For 8-puzzle, the \c StateT is \c std::array, the \c ExpandCostT is \c int.
 */
template <class StateT, class NodeT, typename ExpandCostT>
class GeneralSearcher
{
public:
    // Use a custom priority queue, since it's impossible to dynamically change
    // the comparison function using the one in STL.
    typedef PriorityQueue<NodeT>                            QueueT;
    //! Refer to \ref PriorityQueue::ComparatorT.
    typedef typename QueueT::ComparatorT                    QueueComparatorT;
    typedef ExpandResult<StateT, NodeT, ExpandCostT>        ExpandResultT;
    typedef OperationResult<StateT, ExpandCostT>            OperationResultT;
    /**
     * \brief The queuing function type.
     *
     * It should be the following form:
     * \code
     * void queuingFunciton(QueueT& queue, ExpandResultT result)
     * {
     *     ...
     * }
     * \endcode
     */
    typedef std::function<void(QueueT&, ExpandResultT)>     QueuingFunctionT;
    typedef SearchResult<NodeT>                             SearchResultT;
    /**
     * \brief The type of the function converting \c StateT to \c NodeT.
     *
     * It should be the following form:
     * \code
     * NodeT makeNode(StateT state)
     * {
     *     ...
     * }
     * \endcode
     */
    typedef std::function<NodeT(StateT)>                    NodeMakerT;
    /**
     * \brief The type of the function converting \c NodeT to \c StateT.
     *
     * It should be the following form:
     * \code
     * StateT toState(NodeT node)
     * {
     *     ...
     * }
     * \endcode
     */
    typedef std::function<StateT(NodeT)>                    ToStateT;

private:
    typedef Problem<StateT, ExpandCostT>                    ProblemT;
    typedef typename ProblemT::OperatorT                    OperatorT;
    typedef typename ExpandResultT::ResultVectorT           ExpandResultVectorT;

private:
    // Converters between StateT and NodeT.
    NodeMakerT makeNode;
    ToStateT toState;
    // Queue comparator.
    QueueComparatorT queueComparator;

private:
    ExpandResultT expand(NodeT node, std::vector<OperatorT> operators)
    {
        ExpandResultVectorT result;
        for (auto action : operators)
        {
            auto res = action(toState(node));
            // Only expand nodes on which the operation succeeded.
            if (res.isSucceeded())
                result.push_back(std::make_pair(res.getState(), res.getCost()));
        }
        return ExpandResultT(node, result);
    }

public:
    /**
     * \param makeNode The function converts \c StateT to \c NodeT.
     * \param toState The function converts \c NodeT to \c StateT.
     * \param queueComparator The comparator used in PriorityQueue.
     */
    GeneralSearcher(NodeMakerT makeNode, ToStateT toState, QueueComparatorT queueComparator)
        : makeNode(makeNode), toState(toState), queueComparator(queueComparator) {}

    //! <tt>function general-search(problem, QUEUEING-FUNCTION)</tt>
    SearchResultT generalSearch(ProblemT* problem, QueuingFunctionT queueingFunction)
    {
        // nodes = MAKE-QUEUE(MAKE-NODE(problem, INITIAL-STATE)
        QueueT nodes(queueComparator);  
        nodes.push(makeNode(problem->getInitialState())); // Enqueue initial state

        while (true)
        {
            // if EMPTY(nodes) then return "failure"
            if (nodes.isEmpty()) 
                return SearchResultT::Failure();

            // node = REMOVE-FRONT(nodes)
            auto node = nodes.top();
            nodes.pop();

            // if problem.GOAL-TEST(node.STATE) succeeds then return node
            if (problem->goalTest(toState(node)))
                return SearchResultT::Success(node);

            // nodes = QUEUING-FUNCTION(nodes, EXPAND(node, problem.OPERATORS))
            queueingFunction(nodes, expand(node, problem->getOperators()));
        }
    }
};

#endif
