#include <Rcpp.h>
#include "../include/pruner.h"
using namespace Rcpp;

typedef std::vector< double > dvec;

class pruner::FunArgs {
public:
  // Annotations
  pruner::v_uint a;
  
  // Possible states with a single function
  std::vector< dvec > Pr;
  
  // Model parameters
  dvec pars;
  
  FunArgs(pruner::v_uint a_) {
    
    a = a_;
    Pr = *(new std::vector< dvec >(2));
    Pr[0] = *(new dvec(a.size()));
    Pr[1] = *(new dvec(a.size()));
    
    return;
    
  };
  
  void reset_pr() {
    
    std::fill(Pr[0].begin(), Pr[0].end(), 1.0);
    std::fill(Pr[1].begin(), Pr[1].end(), 1.0);
    return;
    
  };
  ~FunArgs() {};
};

void aphylo(
    std::shared_ptr< pruner::FunArgs >  args,
    pruner::Tree *                      tree,
    pruner::TreeIterator *              iter
) {
  
  if (tree->is_leaf(iter->id())) {
    
  }
  
}