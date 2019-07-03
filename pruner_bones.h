#include <Rcpp.h>
using namespace Rcpp;

#ifndef H_PRUNER_BONES
#define H_PRUNER_BONES

// #define DEBUG_TREE 

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
class FunArgs;

typedef unsigned int uint;
typedef std::vector< uint > v_uint;
typedef std::vector< v_uint > vv_uint;
typedef std::vector< bool > v_bool;

class Tree {
  
private:
  bool is_dag_(int i = -1, int caller = -1, bool up_search = false);
  void postorder_(uint i);
  
protected:
  vv_uint parents;
  vv_uint offspring;
  
  // Auxiliar variables
  v_bool visited;
  v_uint visit_counts;
  uint current_node = 0u;
  
  // Constant
  uint N_NODES;
  uint N_EDGES;
  v_uint POSTORDER;
  
  friend class FunArgs;
  
public:
  // This is public as users can modify it at will
  FunArgs *args;
  std::function<void(Tree*)> fun;
  void eval_fun() {
    fun(this);
  };
  
  // Creation ------------------------------------------------------------------
  ~Tree() {};
  Tree() {};
  Tree(const v_uint & parents_, const v_uint & offspring_, uint & out);
  
  // Getter --------------------------------------------------------------------
  
  // As pointers
  const v_uint * get_parents_of(uint i);
  const v_uint * get_offspring_of(uint i);
  
  // As data
  vv_uint get_parents()   const {return this->parents;};
  vv_uint get_offspring() const {return this->offspring;};
  v_uint get_postorder()  const {return this->POSTORDER;};
  uint n_nodes()          const {return this->N_NODES;};
  uint n_edges()          const {return this->N_EDGES;};
  vv_uint get_edgelist()  const;
  
  void print(bool details = true) const;
  
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
  
  // Postorder -----------------------------------------------------------------
  void postorder();
  
};


#endif