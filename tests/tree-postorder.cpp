// #include <iostream>
// #include <vector>
// #include "../include/pruner.h"

// For creating the MAIN
// #define CATCH_CONFIG_MAIN 
// #include "catch.hpp"

// int main () {
TEST_CASE("Post order sequences respects all", "[postorder]") {
  
  // Tree 1 {1-0, 3-1, 2-1} : Multiple parents --------------------------------
  std::vector< unsigned int > source = {1u, 2u, 3u};
  std::vector< unsigned int > target = {0u, 1u, 1u};

  // Initialization of a tree object
  unsigned int res;
  pruner::Tree tree1(source, target, res);
  
  // Looking at the data
  tree1.print(false);
  std::vector< unsigned int > po = tree1.get_postorder();
  print(po);         

  // Tree 2 {1-0, 3-1, 2-1} : Multiple parents ---------------------------------
  source = {2u, 2u, 3u};
  target = {0u, 1u, 2u};

  // Initialization of a tree object
  pruner::Tree tree2(source, target, res);
  
  // Looking at the data
  tree2.print(false);
  po = tree2.get_postorder();
  print(po);    
  
  // Tree 3 {1-0, 3-1, 2-1} : Multiple parents ---------------------------------
  source = {2u, 2u, 3u, 4u};
  target = {0u, 1u, 2u, 2u};

  // Initialization of a tree object
  pruner::Tree tree3(source, target, res);
  
  // Looking at the data
  tree3.print(false);
  po = tree3.get_postorder();
  print(po);    
 
  // Tree 4 {1-0, 3-1, 2-1} : Multiple parents ---------------------------------
  source = {2u, 2u, 3u, 4u, 5u, 5u};
  target = {0u, 1u, 2u, 2u, 3u, 4u};

  // Initialization of a tree object
  pruner::Tree tree4(source, target, res);
  
  // Looking at the data
  tree4.print(false);
  po = tree4.get_postorder();
  print(po);    

  REQUIRE(tree1.get_postorder().size() == 4u);
 
  // return 0;
  pruner::TreeIterator myiter(&tree1);
  myiter.top();
  REQUIRE(myiter.id() == *myiter);
  
// }
}
