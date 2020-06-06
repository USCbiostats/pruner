// #include <iostream>
// #include <vector>
// #include "../include/pruner.hpp"
// // For creating the MAIN
// #define CATCH_CONFIG_MAIN
// #include "catch.hpp"

// Need to predefined this object
class TreeData {
public:
  ~TreeData() {};
  TreeData(std::vector< double > attrs_) : attrs(attrs_), total(0.0)  {};
  std::vector< double > attrs;
  bool odd = true;
  double total;
};

// Creating the function
void myfun(
    TreeData * args,
    pruner::TreeIterator<TreeData> & titer
) {
  
  std::cout << "I'm called at " << titer.id() << " (postorder)\n";
  
  if (args->odd) {
    args->total /= args->attrs[titer.id()];
  } else {
    args->total *= args->attrs[titer.id()];
  }
  
  args->odd = !args->odd;
  return;
  
}

void myfun2(
    TreeData * args,
    pruner::TreeIterator<TreeData> & titer
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
  pruner::Tree<TreeData> tree(source, target, res);
  std::vector< double > values = {0.1, 0.2, 0.3, 0.4};
  tree.args = new TreeData(values);
  tree.fun  = myfun;
  
  // Calling the pruning algo
  tree.args->total = 1.0;
  tree.prune_postorder();
  
  REQUIRE(tree.args->total == 1.0 / .1 * .2 / .3 * .4);
  
  // Reprune
  double totalcurr = tree.args->total;
  tree.args->odd   = true;
  tree.prune_postorder();
  REQUIRE(tree.args->total == (totalcurr / .1 * .2 / .3 * .4));
  
  // Now repeating the thing but with preorder
  tree.args->total = 1.0;
  tree.args->odd   = true;
  
  // Calling the pruning algo
  tree.prune_preorder();
  
  REQUIRE(tree.args->total == (1.0 / .4 * .3 / .2 * .1));  
  totalcurr = tree.args->total;
  tree.args->odd   = true;
  tree.prune_preorder();
  REQUIRE(tree.args->total == (totalcurr / .4 * .3 / .2 * .1));
  
  // Checking if messing with the pruning seq
  pruner::v_uint oldpseq = tree.get_postorder();
  pruner::v_uint newpseq = {0u, 1u, 2u};
  res = tree.set_postorder(newpseq);
  tree.args->total = 1.0;
  tree.args->odd   = true;
  REQUIRE(tree.get_postorder() == newpseq);
  tree.prune_postorder();
  REQUIRE(tree.args->total == (1.0 / .1 * .2 / .3));
  
  delete tree.args;
  tree.args = nullptr;
  
}

