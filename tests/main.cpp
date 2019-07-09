

#include <vector>
#include <iostream>
#include <type_traits>
#include "../include/pruner.h"

template <class T>
inline void print(std::vector< T > & x) {
  std::cout << "[ " ;
  for (auto i = x.begin(); i != x.end(); ++i)
    std::cout << *i << " ";
  
  std::cout << "]\n";
  
  return;
}

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tree-nodag.cpp"
#include "tree-postorder.cpp"
#include "tree-pruner_postorder.cpp"

