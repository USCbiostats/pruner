#include <Rcpp.h>
#include "pruner.h"  

using namespace Rcpp; 

// Users work from here on -----------------------------------------------------

// Need to declare this class
class pruner::FunArgs {
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

void myfunction(
    std::shared_ptr<pruner::FunArgs> a,
    pruner::Tree * t,
    pruner::TreeIterator * iter) {
  
  // Moving a single step up
  printf("Currently sitting on the node %i.\nCurrent parents are: ", iter->id());
  for (pruner::v_uint::const_iterator i = iter->begin_par(); i != iter->end_par(); ++i) {
    printf(" %i", *i);
  }
  printf("\n");
  
  iter->up();
  printf("Currently sitting on the node %i.\nCurrent parents are: ", iter->id());
  for (pruner::v_uint::const_iterator i = iter->begin_par(); i != iter->end_par(); ++i) {
    printf(" %i", *i);
  }
  printf("\n");
  return;
}

// [[Rcpp::export]]
List fancytree(const pruner::v_uint & parents, const pruner::v_uint & offspring) {
  
  // Creating the object
  uint ans;
  pruner::Tree mytree(parents, offspring, ans);
  
  // Adding function arguments
  mytree.args = std::make_shared< pruner::FunArgs >(1);
  mytree.fun = myfunction;
  
  // Calling functions
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
# set.seed(8)

# Is it working?
tree <-
  structure(
    list(
      edge = structure(c(4L, 5L, 5L, 4L, 5L, 1L, 2L,
                         3L), .Dim = c(4L, 2L)),
      tip.label = c("t3", "t2", "t1"),
      edge.length = c(
        0.718927504029125,
        0.290873386897147,
        0.932269813260064,
        0.769146954407915
      ),
      Nnode = 2L
    ),
    class = "phylo",
    order = "cladewise"
  )
ans <- fancytree(tree$edge[,1] - 1, tree$edge[,2] - 1)

# plot(tree, show.node.label = TRUE)
# cbind(1:length(unlist(ans[[1]])), unlist(ans[[1]]) + 1)
# 
# # DAGS NO DAGS
# invisible(fancytree(tree$edge[,1] - 1, tree$edge[,2] - 1))
# E <- tree$edge
# E <- rbind(E, rev(E[3,]))
# E - 1L
# fancytree(E[,1] - 1, E[,2] - 1) # NOT A DAG
# 
# library(igraph)
# plot(graph_from_edgelist(E), vertex.label = 0:4, vertex.size = 40)

*/