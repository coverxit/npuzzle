/**
 * \file PriorityQueue.hpp
 * \brief A priority queue based on \c std::vector.
 */
#ifndef __PRIORITY_QUEUE__
#define __PRIORITY_QUEUE__

/**
 * \brief A priority queue based on \c std::vector, using STL heap to maintain it.
 *
 * Supports changing comparison function dynamically,
 * which is not supported by the STL version.
 *
 * \tparam ValueT The value type to be used in queue.
 */
template <class ValueT>
class PriorityQueue
{
public:
    //! Use a \c std::vector as the underlying container
    typedef std::vector<ValueT>                               ContainerT;
    typedef typename ContainerT::const_reference              ConstRefT;
    typedef typename ContainerT::size_type                    SizeT;
    /**
     * \brief The comparaison function used in the priority queue.
     *
     * It should be the following form:
     * \code
     * bool comparator(const ValueT& a, const ValueT& b)
     * {
     *     ...
     * }
     * \endcode
     */
    typedef std::function<bool(const ValueT&, const ValueT&)> ComparatorT;

private:
    ContainerT container;
    ComparatorT comparator;

public:
    /**
     * \brief Create a priority queue using \c std::greater
     * as the default comparator.
     *
     * It actually constructs a min-heap rather than
     * a max-heap that STL version does.
     */
    PriorityQueue() : comparator(std::greater<ValueT>()) {}
    /**
     * \brief Create a priority queue with custom comparator.
     * \param comparator The custom comparator.
     */
    PriorityQueue(ComparatorT comparator) : comparator(comparator) {}

    //! Set the comparator.
    void setComparator(ComparatorT comparator) { this->comparator = comparator; }
    //! Get the comparator.
    ComparatorT getComparator() const { return this->comparator; }

    /**
     * \brief Whether the queue is empty.
     * \return True if empty, false if not.
     */
    bool isEmpty() const { return container.empty(); }
    //! The queue size.
    SizeT size() const { return container.size(); }
    //! Get the top element.
    ConstRefT top() const { return container.front(); }

    /**
     * \brief Push an element into queue.
     * \param value The element to be pushed.
     */
    void push(const ValueT& value)
    { 
        container.push_back(value);
        std::push_heap(container.begin(), container.end(), comparator);
    }

    //! Pop the top element from the queue.
    void pop()
    {
        std::pop_heap(container.begin(), container.end(), comparator);
        container.pop_back();
    }
};

#endif