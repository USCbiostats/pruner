#include <iostream>
#include <vector>
#include "../include/pruner.hpp"

int main () {
  
  // Fake tree data
  std::vector< unsigned int > source = {0u, 0u, 1u, 2u, 3u};
  std::vector< unsigned int > target = {1u, 5u, 2u, 3u, 4u};

  // Initialization of a tree object
  unsigned int res;
  pruner::Tree<> tree(source, target, res);
  
  // Looking at the data
  tree.print();         
  
  
  return 0;
  
}
