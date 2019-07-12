#include <Rcpp.h>
#include "../include/pruner.hpp"
#include "03-pruning.hpp"
using namespace Rcpp;


class pruner::TreeData {

public:
  
  uint nstates;
  uint n;
  uint nfuns;
  
  // Annotations
  pruner::vv_uint A;
  pruner::v_uint Ntype;
  
  // Temporal storage ----------------------------------------------------------
  vv_uint states;
  vv_dbl PSI, MU;
  vv_dbl Pr;
  v_dbl Pi;
  
  // Model parameters
  v_dbl mu, psi, eta;
  double pi;
  double ll;
  
  void  set_mu(const v_dbl & mu) {prob_mat(mu, this->MU);return;}
  void set_psi(const v_dbl & psi) {prob_mat(psi, this->PSI);return;}
  void set_eta(const v_dbl & eta) {this->eta = eta;return;}
  void  set_pi(double pi) {root_node_pr(this->Pi, pi, states);return;}
  
  // Destructor and constructor ------------------------------------------------
  ~TreeData() {};
  TreeData(vv_uint A, v_uint Ntype) {
    
    // Initializing data
    this->A       = A;
    this->Ntype   = Ntype;
    
    // Getting meta info, and initializing containers
    this->nfuns   = A[0].size();
    this->n       = A.size();
    this->states  = states_mat(this->nfuns);
    this->nstates = this->states.size();
    this->Pr      = new_vector_array(this->n, this->nstates, 1.0);
    
    // Initializing parameter containers
    mu.resize(2u, 0.0);
    psi.resize(2u, 0.0);
    eta.resize(2u, 0.0);
    Pi.resize(nstates, 0.0);
    pi = 0.0;
    
    MU  = prob_mat(mu);
    PSI = prob_mat(psi);

    ll = 0.0;
    
  };
};

void aphylo(
    pruner::sptr_treedata D,
    pruner::TreeIterator & n
) {
  
  if (n.is_leaf()) {
    
    // Iterating through the states
    for (uint s = 0u; s < D->states.size(); ++s) {
      
      // Throught the functions
      D->Pr[*n][s] = 1.0; // Initializing
      for (uint p = 0u; p < D->nfuns; ++p) {
        
        // ETA PARAMETER
        if (D->A[*n][p] == 9u) {
          
          D->Pr[*n][s] *=
            (1.0 - D->eta[0u]) * D->PSI[D->states[s][p]][0u] +
            (1.0 - D->eta[1u]) * D->PSI[D->states[s][p]][1u]
            ;
          
        } else {
          D->Pr[*n][s] *= D->PSI[D->states[s][p]][D->A[*n][p]]*
            D->eta[D->A[*n][p]];
        }
          
      }
      
    }
    
  } else {
    
    v_uint::const_iterator o_n;
    uint p, s_n, p_n;
    double offspring_ll, s_n_sum;
    
    // Looping through states
    for (uint s = 0u; s < D->nstates; ++s) {
      
      // Now through offspring
      D->Pr[*n][s] = 1.0;
      for (o_n = n.begin_off(); o_n != n.end_off(); ++o_n) {
        
        // Offspring state integration
        offspring_ll = 0.0;
        for (s_n = 0u; s_n < D->nstates; ++s_n) {
          
          s_n_sum = 1.0;
          for (p_n = 0u; p_n < D->nfuns; ++p_n)
            s_n_sum *= D->MU[D->states[s][p]][D->states[s_n][p]];
          
          // Multiplying by off's probability
          offspring_ll += (s_n_sum) * D->Pr[*o_n][s_n];
          
        }
        
        // Getting the joint conditional.
        D->Pr[*n][s] *= offspring_ll;
        
      }
      
    }
    
    // Computing the joint likelihood
    if (*n == n.back()) {
      D->ll = 0.0;
      for (uint s = 0; s < D->nstates; ++s) 
        D->ll += D->Pi[s] * D->Pr[*n][s];
    }
    
  }
  
  
  return;
  
}

// Now the fun begins ----------------------------------------------------------

// [[Rcpp::export]]
SEXP tree_new(
    const vv_uint & edgelist,
    const vv_uint & A,
    const v_uint & Ntype
) {
  
  // Initializing the tree
  uint res;
  Rcpp::XPtr< pruner::Tree > xptr(new pruner::Tree(edgelist[0], edgelist[1], res), true);
  xptr->print(false);
  
  xptr->args = std::make_shared< pruner::TreeData >(A, Ntype);
  xptr->fun  = aphylo;
  
  return wrap(xptr);
}

// [[Rcpp::export]]
SEXP tree_print(SEXP tree_ptr) {
  
  Rcpp::XPtr< pruner::Tree > p(tree_ptr);
  p->print();
  
  return tree_ptr;
  
}

// [[Rcpp::export]]
List tree_ll(
    SEXP tree_ptr,
    const v_dbl & mu,
    const v_dbl & psi,
    const v_dbl & eta,
    const double & pi
) {
  Rcpp::XPtr< pruner::Tree > p(tree_ptr);
  
  // Setting the parameters
  p->args->set_mu(mu);
  p->args->set_psi(psi);
  p->args->set_pi(pi);
  p->args->set_eta(eta);
  
  p->prune_postorder();
  
  return List::create(
    _["Pr"] = wrap(p->args->Pr),
    _["ll"] = wrap(p->args->ll)
  );
}


/***R
set.seed(1)
dat <- aphylo::raphylo(50)

A <- rbind(dat$tip.annotation, dat$node.annotation)[,1]

mu  <- c(.1, .05)
psi <- c(.2, .07)
eta <- c(.8, .9)
Pi  <- .5

tree_ptr <- tree_new(
  edgelist = with(dat$tree, list(edge[,1] - 1, edge[,2] - 1)),
  A        = as.list(A),
  Ntype    = A
)

aphylo_ll <- aphylo::LogLike

microbenchmark::microbenchmark(
  new = tree_ll(tree_ptr, mu = mu, psi = psi, eta = eta, pi = Pi),
  old = aphylo_ll(dat, psi = psi, mu = mu, Pi = Pi, eta = eta),
  times = 100, unit = "relative"
)


*/
