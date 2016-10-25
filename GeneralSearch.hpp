/**
 * \file GeneralSearch.hpp
 * \brief The classes to define a (abstract) general searcher.
 */
#ifndef __GENERAL_SEARCH__
#define __GENERAL_SEARCH__

/**
 * \brief The result of operations in a general problem.
 *
 * OperationResult could only be created by its two static creators
 * OperationResult::Failure and OperationResult::Success.
 *
 * Take 8-Puzzle as example, the \c StateT should be an \c std::array,
 * and the \c ExpandCostT should be \c int.
 *
 * The class should represent the result of moving the blank tile
 * up, down, left and right.
 *
 * \tparam StateT The state type used in a Problem
 * \tparam ExpandCostT The expanding cost type used in a Problem
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
 * \brief A general (abstract) problem class.
 *
 * For 8-puzzle, the \c StateT is a \c std::array and \c ExpandCostT is \c int.
 *
 * \tparam StateT The state type.
 * \tparam ExpandCostT The expanding cost type of operations.
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

private:
    StateT initialState;

public:
    /**
     * \param initialState The initial state.
     * \return An instance of a class derived from Problem.
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
 *
 * It stores the final node with queue node type \c NodeT.
 *
 * SearchResult could only be created by its static creators
 * SearchResult::Failure and SearchResult::Success.
 *
 * \tparam NodeT The node type used in a PriorityQueue.
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
 *
 * For 8-puzzle, the \c StateT is a \c std::array.
 *
 * \tparam StateT The state type used in a Problem.
 * \tparam NodeT The node type used in a GeneralSearcher.
 */
template <class StateT, class NodeT>
class ExpandResult
{
public:
    typedef std::vector<StateT> ExpandedStateVectorT;

private:
    NodeT currentNode;
    ExpandedStateVectorT expandedStates;

public:
    /**
     * \brief Create a expanding result.
     * \param expandedNode The node expanded.
     * \param expandedStates The states expanded from the node.
     */
    ExpandResult(NodeT expandedNode, ExpandedStateVectorT expandedStates) :
        currentNode(expandedNode), expandedStates(expandedStates) {}

    //! Get the expanded node.
    NodeT getCurrentNode() const { return currentNode; }
    //! Get states expanded from the node.
    ExpandedStateVectorT getExpandedState() const { return expandedStates; }
};

/**
 * \brief The general searcher
 *
 * For 8-puzzle, the \c StateT is \c std::array, the \c ExpandCostT is \c int.
 *
 * \tparam StateT The state type used in a Problem.
 * \tparam NodeT The node type used in a PriorityQueue.
 * \tparam ExpandCostT The expanding cost type used in a Problem.
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
    typedef ExpandResult<StateT, NodeT>                     ExpandResultT;
    typedef OperationResult<StateT, ExpandCostT>            OperationResultT;
    /**
     * \brief The queuing function type.
     *
     * It should be the following form:
     * \code
     * QueueT queuingFunciton(QueueT queue, ExpandResultT result)
     * {
     *     ...
     * }
     * \endcode
     */
    typedef std::function<QueueT(QueueT, ExpandResultT)>    QueuingFunctionT;
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
    typedef typename ExpandResultT::ExpandedStateVectorT    ExpandedStateVectorT;

private:
    // Converters between StateT and NodeT.
    NodeMakerT makeNode;
    ToStateT toState;
    // Queue comparator.
    QueueComparatorT queueComparator;

private:
    ExpandResultT expand(NodeT node, std::vector<OperatorT> operators)
    {
        ExpandedStateVectorT states;
        for (auto move : operators)
        {
            auto res = move(toState(node));
            // Only expand nodes on which operations succeeded.
            if (res.isSucceeded())
                states.push_back(res.getState());
        }
        return ExpandResultT(node, states);
    }

public:
    /**
     * \param makeNode The function converts \c StateT to \c NodeT.
     * \param toState The function converts \c NodeT to \c StateT.
     * \param queueComparator The comparator used in PriorityQueue.
     * \return An instance of a class derived from GeneralSearcher.
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
            nodes = queueingFunction(nodes, expand(node, problem->getOperators()));
        }
    }
};

#endif
