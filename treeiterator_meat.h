#include "treeiterator_bones.h"
#include "tree_bones.h"

inline TreeIterator::TreeIterator(Tree * tree) {
  
  this->current_node = tree->POSTORDER.at(0);
  this->pos_in_pruning_sequence = 0u;
  this->tree = tree;
  
  return;
  
}

inline v_uint::const_iterator TreeIterator::begin_off() const {
  
  return (this->tree)->offspring.at(this->current_node).begin();
  
}

inline v_uint::const_iterator TreeIterator::end_off() const {
 
  return (this->tree)->offspring.at(this->current_node).end();
  
}

inline v_uint::const_iterator TreeIterator::begin_par() const {
  
  return (this->tree)->parents.at(this->current_node).begin();
  
}

inline v_uint::const_iterator TreeIterator::end_par() const {
  
  return (this->tree)->parents.at(this->current_node).end();
  
}

// Return codes:
// 0: At the requested point
// 1: End of road.
inline int TreeIterator::up() {
  
  if (++this->pos_in_pruning_sequence == this->tree->N_NODES) {
    --this->pos_in_pruning_sequence;
    return 1;
  }
  
  this->current_node = this->tree->POSTORDER[this->pos_in_pruning_sequence];
  return 0;
  
}

inline int TreeIterator::down() {
  
  if (--this->pos_in_pruning_sequence == 0u) {
    ++this->pos_in_pruning_sequence;
    return 1;
  }
  
  this->current_node = this->tree->POSTORDER[this->pos_in_pruning_sequence];
  return 0;
  
}