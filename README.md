[![Build Status](https://travis-ci.org/USCbiostats/pruner.svg?branch=master)](https://travis-ci.org/USCbiostats/pruner)

# pruner: Implementing the Felsenstein's Tree Pruning algorithm

This C++ template library provides classes to implement Felsenstein tree-pruning
algorithm for efficiently computing likelihood functions on phylogenies.

The library reads in a tree object in the form of `std::vector< unsigned int >`
specifying the source and target of each dyad (edge) and allows the user to 
store arbitrary arguments using memory pointers, and `std::function` to be
called with those arbitrary arguments and tree structure data.

Trees are stored as two lists: each nodes' offspring, and each nodes' parents,
which can be accessed at any point using `TreeIterator` class that implements
tree traversals (pre and post order for pruning).

