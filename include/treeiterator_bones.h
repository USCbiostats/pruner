// #include "typedefs.h"

#ifndef H_PRUNER_TREEITERATOR_BONES
#define H_PRUNER_TREEITERATOR_BONES

class Tree;

class TreeIterator {
  
private:
  uint current_node;
  uint pos_in_pruning_sequence;
  Tree* tree;
  friend class Tree;
public:
  
  ~TreeIterator() {};
  TreeIterator() {};
  TreeIterator(Tree * tree);
  v_uint::const_iterator begin_off() const;
  v_uint::const_iterator end_off() const;
  
  v_uint::const_iterator begin_par() const;
  v_uint::const_iterator end_par() const;
  
  int up();
  int down();
  
  int operator++();
  int operator--();
  
  void top();
  void bottom();
  
  uint id() const {return current_node;};
  
};


#endif