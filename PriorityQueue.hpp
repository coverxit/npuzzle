#ifndef __PRIORITY_QUEUE__
#define __PRIORITY_QUEUE__

// A priority queue based on std::vector, using STL heap to maintain it.
// Supports changing comparison function dynamiclly,
// which is not supported by the STL version.
template <class ValueT>
class PriorityQueue
{
public:
    // Use a vector as the underlying container
    typedef std::vector<ValueT>                               ContainerT;
    typedef typename ContainerT::const_reference              ConstRefT;
    typedef typename ContainerT::size_type                    SizeT;
    typedef std::function<bool(const ValueT&, const ValueT&)> ComparatorT;

private:
    ContainerT container;
    ComparatorT comparator;

public:
    PriorityQueue() {}
    PriorityQueue(ComparatorT comparator) : comparator(comparator) {}

    void setComparator(ComparatorT comparator) { this->comparator = comparator; }
    ComparatorT getComparator() const { return this->comparator; }

    bool isEmpty() const { return container.empty(); }
    SizeT size() const { return container.size(); }
    ConstRefT top() const { return container.front(); }

    void push(const ValueT& v) 
    { 
        container.push_back(v); 
        std::push_heap(container.begin(), container.end(), comparator);
    }

    void pop()
    {
        std::pop_heap(container.begin(), container.end(), comparator);
        container.pop_back();
    }
};

#endif