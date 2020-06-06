// #include <iostream>
// #include <vector>
// #include "../include/pruner.hpp"
// For creating the MAIN
//#define CATCH_CONFIG_MAIN 
//nclude "catch.hpp"

// int main () {
TEST_CASE("DAG no DAG", "[dag]") {
  
  // Tree 1 {1-0, 3-1, 2-1} : Multiple parents --------------------------------
  std::vector< unsigned int > source = {3u, 3u};
  std::vector< unsigned int > target = {0u, 1u};

  // Initialization of a tree object
  unsigned int res;
  pruner::Tree<> tree(source, target, res);
  
  REQUIRE(tree.is_connected() == false);
  REQUIRE(tree.is_dag() == true);
  
}
