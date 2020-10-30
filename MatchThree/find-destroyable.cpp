#include "find-destroyable.hpp"

#include "scene-singletone.hpp"
#include "field-singletone.hpp"
#include "utils.hpp"
#include "node.hpp"

void find_destroyable::operator ()(const Node * node) const
{
  if (!node) {
    throw std::runtime_error("Unexpected error");
  }

  if (node->getDestroyFlag()) {
    size_t index = node->getIndex();

    if (index + 1 != ((index / parameters::FIELD_SIZE) + 1) * parameters::FIELD_SIZE
        && FieldSingletone::instance()->at(index + 1)->getDestroyFlag()) {

      return;
    }

    size_t count = 0;
    int i = static_cast< int >(index);
    while (i + 1 != ((index / parameters::FIELD_SIZE)) * parameters::FIELD_SIZE
        && FieldSingletone::instance()->at(i)->getDestroyFlag()) {

      ++count;
      --i;
    }

    for (int i = static_cast< int >(index - count); i > static_cast< int >(index - index % parameters::FIELD_SIZE - 1); --i) {
      FieldSingletone::instance()->at(i)->moveToPos(FieldSingletone::instance()->at(i + count)->getHomePos());
      nodeFuncs::swap(i, i + count);
    }

    index = (index / parameters::FIELD_SIZE) * parameters::FIELD_SIZE;

    nodeFuncs::addToScore(count);

    while(count) {
      --count;
      SceneSingletone::instance()->removeItem(FieldSingletone::instance()->at(index + count));
    }
  }
}
