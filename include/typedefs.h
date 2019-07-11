
#ifdef PRUNER_DEBUG_ON
template <class T>
inline void print_vector(const std::vector< T > & V) {
  
  for (int i = 0; i < V.size(); ++i)
    std::cout << "[" << (V.at(i)) << "]\n";
  
  return;
  
}

#endif

#ifndef H_PRUNER_TYPEDEFS
#define H_PRUNER_TYPEDEFS

//! @file typedefs.h

//! Unsigned integer
typedef unsigned int uint;

//! A vector of unsigned integers
typedef std::vector< uint > v_uint;

//! A vector of unsigned integer vectors
typedef std::vector< v_uint > vv_uint;

//! A vector of logicals
typedef std::vector< bool > v_bool;

class FunArgs;
//! Shorthand for shared_ptr< FunArgs >
typedef std::shared_ptr< FunArgs > sptr_args;

// Auxiliar functions ----------------------------------------------------------
template <class T>
inline uint find_in_vector(const std::vector< T >* x, T value) {
  
  for (uint i = 0u; i < x->size(); ++i)
    if (x->at(i) == value) {
      return i;
    }
    
  return 0;
}

#endif