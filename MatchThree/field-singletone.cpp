#include "field-singletone.hpp"

#include <memory>

static std::unique_ptr< std::vector< Node * > > nodes;

std::vector< Node * > * FieldSingletone::instance()
{
  if (!nodes) {
    nodes.reset(new std::vector< Node * >);
  }

  return &(*nodes);
}
