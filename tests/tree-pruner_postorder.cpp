// #include <iostream>
// #include <vector>
// #include "../include/pruner.h"
// For creating the MAIN
// #define CATCH_CONFIG_MAIN
// #include "catch.hpp"

// Need to predefined this object
class pruner::FunArgs {
public:
  ~FunArgs() {};
  FunArgs(std::vector< double > attrs_) : attrs(attrs_), total(0.0)  {};
  std::vector< double > attrs;
  double total;
};

// Creating the function
void myfun1(
    std::shared_ptr< pruner::FunArgs > args,
    pruner::TreeIterator & titer
) {
  
  std::cout << "I'm called at " << titer.id() << " (postorder)\n";
  args->total += args->attrs[titer.id()];
  
  return;
  
}

void myfun2(
    std::shared_ptr< pruner::FunArgs > args,
    pruner::TreeIterator & titer
) {
  
  std::cout << "I'm called at " << titer.id() << " (preorder)\n";
  args->total /= args->attrs[titer.id()];
  
  return;
  
}

// int main () {
TEST_CASE("Postorder pruner", "[tree][postorder]") {
  
  // Tree 1 {1-0, 3-1, 2-1} : Multiple parents --------------------------------
  std::vector< unsigned int > source = {1u, 2u, 3u};
  std::vector< unsigned int > target = {0u, 1u, 1u};
  
  // Initialization of a tree object
  unsigned int res;
  pruner::Tree tree(source, target, res);
  std::vector< double > values = {0.1, 0.2, 0.3, 0.4};
  tree.args = std::make_shared< pruner::FunArgs >(values);
  tree.fun  = myfun1;
  
  // Calling the pruning algo
  tree.prune_postorder();
  
  REQUIRE(tree.args->total == 1.0);
  
  // Now repeating the thing but with preorder
  tree.args->total = 1.0;
  tree.fun = myfun2;
  
  // Calling the pruning algo
  tree.prune_preorder();
  
  REQUIRE(tree.args->total == (1.0 / .4 / .3 / .2 / .1));  
  
  // Checking
}

