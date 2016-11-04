# N-Puzzle %Problem

## Introduction

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

## Structure

Since the assignment requires the code should be kept as **general** as possible, we first introduce classes needed for a (abstract) general searcher:

* [Problem<StateT, ExpandCostT>](@ref Problem), @copybrief Problem
	
	@copydetails Problem

* [PriorityQueue<ValueT>](@ref PriorityQueue), @copybrief PriorityQueue

	@copydetails PriorityQueue
	
* [GeneralSearcher<StateT, NodeT, ExpandCostT>](@ref GeneralSearcher), @copybrief GeneralSearcher

	@copydetails GeneralSearcher
	
For a specific problem, which is N-Puzzle problem for this project, we defines use a namespace NPuzzle.

Then we set the fundamental arguments:

* NPuzzle::demonstration, @copybrief NPuzzle::demonstration
	
	* NPuzzle::matrixDemonstration, @copybrief NPuzzle::matrixDemonstration
	
* NPuzzle::moveCost, @copybrief NPuzzle::moveCost

In addition, we define some `structs` and `typedefs`:

* NPuzzle::NPuzzleState, @copybrief NPuzzle::NPuzzleState
* NPuzzle::NPuzzleNode, @copybrief NPuzzle::NPuzzleNode
* NPuzzle::NPuzzleCostFunction, @copybrief NPuzzle::NPuzzleCostFunction

Having all these definitions, we can safely define the searcher for N-Puzzle problem:

* NPuzzle::NPuzzleProblem, @copybrief NPuzzle::NPuzzleProblem
* NPuzzle::NPuzzleSearcher, @copybrief NPuzzle::NPuzzleSearcher

For the three kinds of searching alogrithms, we have the following corresponding functions:

* NPuzzle::GetUniformHeuristicCost, @copybrief NPuzzle::GetUniformHeuristicCost
* NPuzzle::GetMisplacedTileCount, @copybrief NPuzzle::GetMisplacedTileCount
* NPuzzle::GetManhattanDistance, @copybrief NPuzzle::GetManhattanDistance

Because the assignment requires recording the total amount of nodes expanded and the max length of the search queue, we construct a solver:

* NPuzzle::NPuzzleSolver, @copybrief NPuzzle::NPuzzleSolver

Now we have all the necessary classes, then we can solve the N-Puzzle problem by calling NPuzzle::NPuzzleSolver::solve() with an `initialState`.