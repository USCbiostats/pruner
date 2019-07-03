#include "pruner.h"  

// Users work from here on -----------------------------------------------------

// Need to declare this class
class FunArgs {
  int nnodes;
public:
  
  ~FunArgs() {};
  FunArgs(int n): nnodes(n) {};
  
  int get_nnodes() {return this->nnodes;};
  void set_nnodes(int n) {
    this->nnodes = n;
    return;
  };
};

void myfunction(Tree * data) {
  printf("We have %d nodes.\n", data->args->get_nnodes());
}

// [[Rcpp::export]]
List fancytree(const v_uint & parents, const v_uint & offspring) {
  
  // Creating the object
  uint ans;
  Tree mytree(parents, offspring, ans);
  
  // Adding function arguments
  mytree.args = new FunArgs(1);
  mytree.fun = myfunction;
  
  // Calling functions
  mytree.fun(&mytree); // Explicit call
  mytree.args->set_nnodes(5);
  mytree.eval_fun();   // Implicit call
  
  // Example printing
  mytree.print();
  
  return List::create(
    _["edgelist"]  = mytree.get_edgelist(),
    _["postorder"] = mytree.get_postorder(),
    _["dag"]       = mytree.is_dag()
  );
  
}

/***R
set.seed(8)

# Is it working?
tree <- ape::rtree(3)
ans <- fancytree(tree$edge[,1] - 1, tree$edge[,2] - 1)

plot(tree, show.node.label = TRUE)
cbind(1:length(unlist(ans[[1]])), unlist(ans[[1]]) + 1)

# DAGS NO DAGS
invisible(fancytree(tree$edge[,1] - 1, tree$edge[,2] - 1))
E <- tree$edge
E <- rbind(E, rev(E[3,]))
E - 1L
invisible(fancytree(E[,1] - 1, E[,2] - 1)) # NOT A DAG

library(igraph)
plot(graph_from_edgelist(E), vertex.label = 0:4, vertex.size = 40)

*/