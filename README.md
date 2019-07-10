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

## Contributing

We welcome contributions to `pruner`. Whether it is reporting a bug,
starting a discussion by asking a question, or proposing/requesting a
new feature, please go by creating a new issue
[here](https://github.com/USCbiostats/pruner/issues) so that we can talk
about it.

Please note that this project is released with a
[Contributor Code of Conduct](https://github.com/USCbiostats/pruner/blob/master/CODE_OF_CONDUCT.md). By participating in this project you agree
to abide by its terms.

## Funding

Supported by National Cancer Institute Grant \#1P01CA196596.