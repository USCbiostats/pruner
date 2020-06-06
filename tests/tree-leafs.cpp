// #include <iostream>
// #include <vector>
// #include "../include/pruner.hpp"
// For creating the MAIN
//#define CATCH_CONFIG_MAIN 
// #include "catch.hpp"

// int main () {
TEST_CASE("Tips are tagged right", "[tip]") {
  
  // Tree
  std::vector< unsigned int > source = {1u, 1u, 3u, 3u};
  std::vector< unsigned int > target = {0u, 3u, 2u, 4u};

  // Initialization of a tree object
  unsigned int res;
  pruner::Tree<> tree(source, target, res);
  
  pruner::v_uint seq = {0u, 2u, 4u};
  REQUIRE(tree.get_tips() == seq);

  // Tree
  source = {1u, 1u, 3u, 3u, 1u, 5u, 6u};
  target = {0u, 3u, 2u, 4u, 5u, 7u, 7u};

  // Initialization of a tree object
  pruner::Tree<> tree2(source, target, res);
  
  seq = {1u, 2u, 2u, 1u};
  REQUIRE(tree2.get_dist_tip2root() == seq);
 
  
}

