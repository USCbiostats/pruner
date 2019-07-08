// #include "tree_bones.h"
// #include "treeiterator_bones.h"
// #include "treeiterator_meat.h"
// #include <memory> // For the std::shared_ptr

#ifndef H_PRUNER_TREE_MEAT
#define H_PRUNER_TREE_MEAT

// Pruning ---------------------------------------------------------------------

// Function to get the pre and post order
inline void Tree::postorder() {
  
  // If it is not initialized
  if (this->POSTORDER.size() == 0u)
    POSTORDER.reserve(this->N_NODES);
  
  // We can start the algorithm from any place
  postorder_(0u);
  
  this->reset_visited();
  
  return;
}

inline void Tree::postorder_(uint i) {
  
  // Setting the state to visited
  this->visited[i] = true;
  
  // First, check the offspring
  for (int j = 0; j < this->offspring[i].size(); ++j) {
    
    // Nothing to do here
    if (this->visited[this->offspring[i][j]])
      continue;
    
    postorder_(this->offspring[i][j]);
    
  }
  
  // After visiting all of its childs, we need to add this node to the pruning
  // sequence and continue with its parent(s).
  POSTORDER.push_back(i);
  for (int j = 0; j < this->parents[i].size(); ++j) {
    
    // Nothing to do here
    if (this->visited[this->parents[i][j]])
      continue;
    
    postorder_(this->parents[i][j]);
    
  }
  
  return;
}

// Returns an edgelist in the form a vector of two uint vectors.
inline vv_uint Tree::get_edgelist() const {
  
  vv_uint res(2u);
  
  // We know the number of edges before hand, so we better save the space
  // up-front.
  res[0].reserve(this->N_EDGES);
  res[1].reserve(this->N_EDGES);
  
  for (int i = 0; i < this->N_NODES; ++i) {
    for (int j = 0; j < this->offspring[i].size(); ++j) {
      res[0].push_back(i);
      res[1].push_back(this->offspring[i][j]);
    }
  }
  
  return res;
  
}

inline void Tree::print(bool details) const {
  
  // Basic information
  printf(
    "tree structure with %i nodes and %i edges.\n",
    this->n_nodes(), this->n_edges()
  );
  
  // When details are true, information regarding the offspring and parents
  // is shown.
  if (!details)
    return;
  
  // What is sais below (just divide the sections :P)
  printf("List of offspring:\n");
  for (int i = 0; i < (int) this->offspring.size(); ++i) {
    
    printf("%4i: [", i);
    
    if (this->offspring[i].size() == 0u)
      printf(" -");
    else {
      v_uint::const_iterator iter;
      for (iter = this->offspring[i].begin(); iter != this->offspring[i].end(); ++iter)
        printf(" %i", *iter);
    }
    
    printf(" ]\n");
    
  }
  
  // What is sais below (just divide the sections :P)
  printf("List of parents:\n");
  for (int i = 0; i < (int) this->parents.size(); ++i) {
    
    printf("%4i: [", i);
    
    if (this->parents[i].size() == 0u)
      printf(" -");
    else {
      v_uint::const_iterator iter;
      for (iter = this->parents[i].begin(); iter != this->parents[i].end(); ++iter)
        printf(" %i", *iter);
    }
    
    printf(" ]\n");
    
  }
  
  return;
  
}

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
  for (uint i = 0; i < m; ++i) {
    this->offspring[parents_[i]].push_back(offspring_[i]);
    this->parents[offspring_[i]].push_back(parents_[i]);
  }
  
  
  // Constants
  this->N_NODES = (uint) maxid + 1u;
  this->N_EDGES = m;
  
  // Generating the postorder sequence
  this->postorder();
  
  // Initializing iterator 
  this->I = TreeIterator(this);
  
  out = 0u;
  return;
  
}

// A recursive function to check whether the tree is a DAG or not. -------------
typedef v_uint::const_iterator v_uint_iter;
inline bool Tree::is_dag() {
  
  bool res = this->is_dag_();
  this->reset_visited();
  
  return res;
  
}


inline bool Tree::is_dag_(int i, int caller, bool up_search) {
  
  // For the first iteration
  if (i < 0) 
    i = 0u, caller = -1;
  
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

// The preorder is just the post order reversed
inline v_uint Tree::get_preorder() const {
  
  v_uint res = this->get_postorder();
  std::reverse(res.begin(), res.end());
  return res;
  
}

#endif