// #include <memory>
// #include "typedefs.h"
// #include "treeiterator_bones.h"

#ifndef H_PRUNER_TREE_BONES
#define H_PRUNER_TREE_BONES

#ifndef MAX_TREE_SIZE
#define MAX_TREE_SIZE 20000
#endif

// Arbtrary set of arguments, this is the class that the creator function should
// inherit. Ideally it should have:
// - Model parameters can should be modifiable by a function
// - Additional data such as matrices and what not
// We start by declaring it, the user later on defines it

/** Arguments to be passed to Tree::fun.
 * 
 */
class FunArgs;
class Tree;
class TreeIterator;


//! Tree class 
/** The Tree class is the core of pruner. The most relevant members are
 * - `parents`
 * - `offspring`
 * - `iterator`
 * - `args`
 * - `fun`
 */
class Tree {
  
private:
  bool is_dag_(int i = -1, int caller = -1, bool up_search = false);
  void postorder_(uint i);
  void postorder();
  TreeIterator I;
  
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
  
  //! Postorder sequence
  /**
   * The POSTORDER vector shows the order in which TreeIterator runs through
   * the tree. This is set at the moment during which the tree is created.
   */
  v_uint POSTORDER;
  
  friend class FunArgs;
  friend class TreeIterator;
  
public:
  
  //! Arbitrary set of arguments
  std::shared_ptr< FunArgs > args;
  
  //! Callable function during the the tree traversal
  /**
   * The idea of this function is that the users can specify what to do 
   * during a call to Tree::prune_postorder and Tree::prune_preorder conditional
   * on the current node. The argument of class TreeIterator allows them to
   * get that information by accessing the member function TreeIterator::id.
   */
  std::function<void(std::shared_ptr< FunArgs >, Tree*, TreeIterator*)> fun;
  
  void eval_fun() {
    fun(this->args, this, &this->I);
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
  v_uint get_preorder()   const;
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
  
  bool is_connected() const {return this->POSTORDER.size() == this->N_NODES;};
  
  // Setters -------------------------------------------------------------------
  void reset_visited() {
    std::fill(this->visited.begin(), this->visited.end(), false);
    std::fill(this->visit_counts.begin(), this->visit_counts.end(), 0u);
    return ;
  };
  
  // Pre-Post/order ------------------------------------------------------------
  
  //! Do the tree-traversal using the postorder
  /** The user defined function `fun` will be called at each step. This allows
   * users implementing their own model.
   */
  void prune_postorder();
  
  //! Do the tree-traversal using the preorder
  /**
   * See Tree::prune_postorder.
   */
  void prune_preorder();
  
  // uint 
  
  /*** The implementation should be as follows:
   * 1. Initialize the algorithm (this can be done once the object is created).
   *    For this to work we need to check that the variable POSTORDER has been
   *    created.
   * 2. Have function next_node(), which essentially moves the head according
   *    to wether we are going with a POSTORDER or PREORDER.
   * 3. Once the pointer has been moved, the user should have the following functions
   *    a. list_offspring
   *    b. list_parent
   *    c. 
   */
  
    
  
};

/** \example 00-hello-world.cpp This shows how to create and print a Tree class
 * object.
 * 
 * \example 01-adding-function.cpp In this example we add a function to the tree
 * (including a pointer to the arguments). This includes the definition of the
 * class FunArgs, which is stored as args in the Tree object.
 * 
 * \example 02-rcpp.cpp Shows how can this library be used within R.
 * 
 */

#endif