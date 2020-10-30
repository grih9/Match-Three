#ifndef FIND_DESTROYABLE_HPP
#define FIND_DESTROYABLE_HPP

class Node;

struct find_destroyable {
  void operator ()(const Node *) const;
};

#endif
