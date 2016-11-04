# N-Puzzle Problem
### CS 170: Assignment 1

Using the following three kinds of alogrithms to solve the N-Puzzle problem:

* Uniform Cost Search.
* A* with the Misplaced Tile heuristic.
* A* with the Manhattan Distance heuristic.

The main "driver" funtion should match this pseudo code:

```
function general-search(problem, QUEUEING-FUNCTION)
	nodes = MAKE-QUEUE(MAKE-NODE(problem.INITIAL-STATE)) 
	loop do
		if EMPTY(nodes) then return "failure" 
			node = REMOVE-FRONT(nodes)
		if problem.GOAL-TEST(node.STATE) succeeds then 
			return node 
		nodes = QUEUEING-FUNCTION(nodes, EXPAND(node, problem.OPERATORS))
	end
```