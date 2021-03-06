#include <iostream>
#include <vector>
#include "../include/pruner.hpp"

// STEP 1: Need to declare this class
class TreeData {
  int nnodes;
public:
  
  ~TreeData() {};
  TreeData(int n): nnodes(n) {};
  
  int get_nnodes() {return this->nnodes;};
  void set_nnodes(int n) {
    this->nnodes = n;
    return;
  };
};

// STEP 2: Define a function to be passed to the algorithm
void myfunction(
    TreeData * a,
    pruner::TreeIterator<TreeData> & iter) {
  
  // Moving a single step up
  printf("Currently sitting on the node %i.\nCurrent parents are: ", iter.id());
  for (auto i = iter.begin_par(); i != iter.end_par(); ++i) {
    printf(" %i", *i);
  }
  printf("\n");
  
  iter.up();
  printf("Currently sitting on the node %i.\nCurrent parents are: ", iter.id());
  for (auto i = iter.begin_par(); i != iter.end_par(); ++i) {
    printf(" %i", *i);
  }
  printf("\n");
  return;
}

int main () {
  
  // Fake tree data
  pruner::v_uint source = {0u, 0u, 1u, 2u, 3u};
  pruner::v_uint target = {1u, 5u, 2u, 3u, 4u};
  
  // Initialization of a tree object
  pruner::uint res;
  pruner::Tree<TreeData> tree(source, target, res);
  
  // Looking at the data
  tree.print();         
  
  // We can pass the function:
  // Adding function arguments
  tree.args = new TreeData(1);
  tree.fun = myfunction;
  
  // Calling functions
  tree.args->set_nnodes(5);
  tree.eval_fun();   // Implicit call
  
  tree.prune_postorder();
  
  delete tree.args;
  tree.args = nullptr;
  
  
  return 0;
  
}
