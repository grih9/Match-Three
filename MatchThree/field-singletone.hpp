#ifndef FIELDSINGLETONE_HPP
#define FIELDSINGLETONE_HPP

#include <vector>

class Node;

class FieldSingletone {
public:
  static std::vector< Node * > * instance();
};

#endif
