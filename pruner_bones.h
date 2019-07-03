#include <Rcpp.h>
using namespace Rcpp;

#define DEBUG_TREE true

#ifdef DEBUG_TREE
template <class T>
void print_vector(const std::vector< T > & V) {
  
  for (int i = 0; i < V.size(); ++i)
    std::cout << "[" << (V.at(i)) << "]\n";
  
  return;
  
}

#endif

// [[Rcpp::plugins(cpp11)]]

#ifndef MAX_TREE_SIZE
#define MAX_TREE_SIZE 20000
#endif



// Arbtrary set of arguments, this is the class that the creator function should
// inherit. Ideally it should have:
// - Model parameters can should be modifiable by a function
// - Additional data such as matrices and what not
// We start by declaring it, the user later on defines it
struct FunArgs {int nnodes;};

typedef unsigned int uint;
typedef std::vector< uint > v_uint;
typedef std::vector< v_uint > vv_uint;
typedef std::vector< bool > v_bool;

class Tree {
  
private:
  bool is_dag_(int i = -1, int caller = -1, bool up_search = false);
  
protected:
  vv_uint parents;
  vv_uint offspring;
  
  // Auxiliar variables
  v_bool visited;
  v_uint visit_counts;
  uint current_node = 0u;
  
  friend struct FunArgs;
  
public:
  // This is public as users can modify it at will
  FunArgs args;
  std::function<void(Tree*)> fun;
  void eval_fun() {
    fun(this);
  };
  
  // Creation ------------------------------------------------------------------
  ~Tree() {};
  Tree() {};
  Tree(const v_uint & parents_, const v_uint & offspring_, uint & out);
  
  // Getter --------------------------------------------------------------------
  const v_uint * get_parents_of(uint i);
  const v_uint * get_offspring_of(uint i);
  vv_uint get_parents() {return this->parents;};
  vv_uint get_offspring() {return this->offspring;};
  
  uint n_nodes() {return parents.size();};
  
  // Checker functions ---------------------------------------------------------
  bool is_dag();
  bool is_root(int i = -1) {
    if (i >= 0)
      i = current_node;
    if (parents[i].size() == 0u)
      return true;
    else 
      return false;
  }
  
  bool is_leaf(int i = -1) { // Has a parent, and doesn't have offspring
    if (i >= 0)
      i = current_node;
    return !this->is_root(i) && offspring[i].size() == 0u;
  }
  
  // Setters -------------------------------------------------------------------
  void reset_visited() {
    std::fill(this->visited.begin(), this->visited.end(), false);
    std::fill(this->visit_counts.begin(), this->visit_counts.end(), 0u);
    return ;
  };
  
};

// Return codes:
// 0: OK
// 1: Sizes of parent and offspring differ
// 2: MAX_TREE_SIZE reached.
inline Tree::Tree(const v_uint & parents_, const v_uint & offspring_, uint & out) {
  
  // If different sizes, then it shouldn't work
  if (parents_.size() != offspring_.size()) {
    out = 1u;
    return;
  }
  
  // Checking ranges
  int maxid = 0, m = (int) parents_.size();
  for (int i = 0; i < m; ++i) {
    if ((parents_[i] > MAX_TREE_SIZE) || (offspring_[i] > MAX_TREE_SIZE)) {
      out = 2u;
      return;
    }
    
    if (maxid < parents_[i])
      maxid = parents_[i];
    if (maxid < offspring_[i])
      maxid = offspring_[i];
  }
  
  // Resizing the vectors
  this->parents.resize(maxid + 1u);
  this->offspring.resize(maxid + 1u);
  
  this->visited.resize(maxid + 1u, false);
  this->visit_counts.resize(maxid + 1u, 0u);
  
  // Adding the data
  for (uint i = 0; i < maxid; ++i) {
    this->offspring[parents_[i]].push_back(offspring_[i]);
    this->parents[offspring_[i]].push_back(parents_[i]);
  }
  
  
  out = 0u;
  return;
  
}

// A recursive function to check whether the tree is a DAG or not. -------------
typedef v_uint::const_iterator v_uint_iter;
inline bool Tree::is_dag() {
  
  // Creating copies to delete stuff
  v_uint noff(this->offspring.size());
  int n = noff.size();
  for (int i = 0; i < n; ++i)
    noff[i] = offspring[i].size();
  
  v_uint  nodes(n);
  for (int i = 0; i < n; ++i)
    nodes[i] = i;
#ifdef DEBUG_TREE
  for (int i = 0; i < n; ++i) {
    printf("Offspring of [%i]\n", i);
    print_vector(this->offspring[i]);
  }
#endif
  
  int cur0 = 0;
  bool change;
  while (cur0 < n) {
    
    // Removing nodes with no offspring
    int i = cur0;
    change = false;
    for (; i < n; ++i) {
      
#ifdef DEBUG_TREE
      printf("---- Current set (i: noffs[nodes[i]]): (%i, %i): -----\n",
             nodes[i], (int) noff[nodes[i]]);
      print_vector(noff);
#endif
      
      // Moving i to the tail and shrinking the pool (index only)
      if (noff[nodes[i]] == 0u) {
        
        // Removing from list of previous individuals
        for (int j = 0; j < (int) this->parents[nodes[i]].size(); ++j)
          --noff[this->parents[nodes[i]][j]];
        
        nodes[i] = nodes[cur0]; // Bumping the last good one to the current state
        // Next time we start from the next
        cur0++;
        change = true;
        break;
      }
      
    }
    
    if (!change)
      break;
    
  }
  
#ifdef DEBUG_TREE
  printf("n: %i\n", cur0);
  print_vector(nodes);
#endif
  
  
  if (cur0 == n)
    return true;
  else
    return false;
  
  
}


inline bool Tree::is_dag_(int i, int caller, bool up_search) {
  
  // For the first iteration
  if (i < 0) 
    i = this->current_node, caller = -1;
  
  // Yes, this is not a dag (came here multiple times)
  if (this->visited[i])
    return false;
  this->visited[i] = true;
  
  // Iterating through parents
  for (v_uint_iter n = this->parents[i].begin(); n != this->parents[i].end(); ++n) {
    
#ifdef DEBUG_TREE
    std::printf(
      "Tree::is_dag() @ parents   (i, caller, *n, up_search): (%i, %i, %i, %i)\n",
      i, caller, *n, up_search
    );
#endif
    
    // Checking 1:1 cycles
    if ((int) *n == caller) {
#ifdef DEBUG_TREE
      std::printf("\tChecking 1:1 cycles.\n");
#endif
      if (up_search) return false;
      else continue; 
    }
    
    if (!(this->is_dag_((int) *n, i, true)))
      return false;
  }
  
  // Iterating through offspring
  for (v_uint_iter n = this->offspring[i].begin(); n != this->offspring[i].end(); ++n) {
    
#ifdef DEBUG_TREE
    std::printf(
      "Tree::is_dag() @ offspring (i, caller, *n, up_search): (%i, %i, %i, %i)\n",
      i, caller, *n, up_search
    );
#endif
    
    // Checking 1:1 cycles
    if ((int) *n == caller) {
#ifdef DEBUG_TREE
      std::printf("\tChecking 1:1 cycles.\n");
#endif
      if (!up_search) return false;
      else continue; 
    }
    
    if (!(this->is_dag_((int) *n, i, false)))
      return false;
  }
  
  return true;
  
}