#include "utils.hpp"

#include <QPainter>
#include <QObject>
#include <QGraphicsItemAnimation>

#include <algorithm>

#include "node.hpp"
#include "field-singletone.hpp"
#include "find-destroyable.hpp"
#include "scene-singletone.hpp"

bool parameters::isDestroyable = false;
bool parameters::isAnimationGoing = false;

size_t parameters::score = 0;

QTimeLine * parameters::timer = new QTimeLine(parameters::ANIMATION_PERIOD_MS);
QTimeLine * parameters::replaceTimer = new QTimeLine(parameters::ANIMATION_PERIOD_MS);

void nodeFuncs::add(Node * obj)
{
  if (!obj) {
    std::cerr << "OBJECT EMPTY\n";
    return;
  }
  if (FieldSingletone::instance()->size() == parameters::FIELD_SIZE * parameters::FIELD_SIZE) {
    std::cerr << "FIELD IS FULL\n";
    return;
  }

  FieldSingletone::instance()->push_back(obj);
}

void nodeFuncs::swap(size_t index1, size_t index2)
{
  if (!FieldSingletone::instance()) {
    return;
  }

  auto it1 = (*FieldSingletone::instance()).begin();
  auto it2 = (*FieldSingletone::instance()).begin();

  std::advance(it1, index1);
  std::advance(it2, index2);

  QPointF position = (*FieldSingletone::instance())[index1]->getHomePos();

  FieldSingletone::instance()->at(index1)->setHomePos(FieldSingletone::instance()->at(index2)->getHomePos());
  FieldSingletone::instance()->at(index2)->setHomePos(position);
  FieldSingletone::instance()->at(index2)->moveToPos(FieldSingletone::instance()->at(index2)->getHomePos());

  FieldSingletone::instance()->at(index2)->setIndex(index1);
  FieldSingletone::instance()->at(index1)->setIndex(index2);

  std::iter_swap(it1, it2);
}

void nodeFuncs::process()
{
  std::for_each(FieldSingletone::instance()->begin(), FieldSingletone::instance()->end(), find_destroyable());

  setDestroyableFlag(false);
}

bool nodeFuncs::getDestroyableFlag()
{
  return parameters::isDestroyable;
}

bool nodeFuncs::getAnimationFlag()
{
  return parameters::isAnimationGoing;
}

void nodeFuncs::setDestroyableFlag(bool flag)
{
  parameters::isDestroyable = flag;
}

void nodeFuncs::setAnimationFlag(bool flag)
{
  parameters::isAnimationGoing = flag;
}

void nodeFuncs::setDestroyFlags(parameters::direction_t direction, size_t number)
{
  switch (direction) {
  case parameters::direction_t::HORIZONTAL:
    {
      size_t streak = 1;
      Qt::GlobalColor currentColor = FieldSingletone::instance()->at(number)->getColor();

      for (size_t i = number + parameters::FIELD_SIZE; i < parameters::FIELD_SIZE * parameters::FIELD_SIZE; i += parameters::FIELD_SIZE ) {
        if (FieldSingletone::instance()->at(i)->getColor() == currentColor) {
          streak++;

          if (streak >= 3) {
            if (streak == 3) {
              FieldSingletone::instance()->at(i - parameters::FIELD_SIZE)->setDestroyFlag(true);
              FieldSingletone::instance()->at(i - 2 * parameters::FIELD_SIZE)->setDestroyFlag(true);
            }
            FieldSingletone::instance()->at(i)->setDestroyFlag(true);
            nodeFuncs::setDestroyableFlag(true);
          }

        } else {
          streak = 1;
          currentColor = FieldSingletone::instance()->at(i)->getColor();
        }
      }
      break;
    }
  case parameters::direction_t::VERTICAL:
    {
      size_t streak = 1;
      size_t index = number * parameters::FIELD_SIZE;
      Qt::GlobalColor currentColor = FieldSingletone::instance()->at(index)->getColor();

      for (size_t i = index + 1; i < index + parameters::FIELD_SIZE; i++) {
        if (FieldSingletone::instance()->at(i)->getColor() == currentColor) {
          streak++;

          if (streak >= 3) {
            if (streak == 3) {
              FieldSingletone::instance()->at(i - 1)->setDestroyFlag(true);
              FieldSingletone::instance()->at(i - 2)->setDestroyFlag(true);
            }
            FieldSingletone::instance()->at(i)->setDestroyFlag(true);
            nodeFuncs::setDestroyableFlag(true);
          }

        } else {
          streak = 1;
          currentColor = FieldSingletone::instance()->at(i)->getColor();
        }
      }
      break;
    }
  default:
    {
    }
  }
}

void nodeFuncs::addToScore(size_t addition)
{
  parameters::score += addition;
}

size_t nodeFuncs::getScore()
{
  return parameters::score;
}

void nodeFuncs::resetScore()
{
  parameters::score = 0;
}

Qt::GlobalColor nodeFuncs::randomiseNodeColor()
{
  int i = rand() % 5;

  switch (i) {
  case 0:
    {
      return Qt::red;
    }
  case 1:
    {
      return Qt::green;
    }
  case 2:
    {
      return Qt::blue;
    }
  case 3:
    {
      return Qt::yellow;
    }
  case 4:
    {
      return Qt::magenta;
    }
  }
}

void nodeFuncs::generateNewNodes()
{
  Qt::GlobalColor prevColor = Qt::black;
  for (size_t i = 0; i < parameters::FIELD_SIZE * parameters::FIELD_SIZE; i += parameters::FIELD_SIZE) {
    size_t index = i;

    while (FieldSingletone::instance()->at(index)->getDestroyFlag()) {

      Qt::GlobalColor color = nodeFuncs::randomiseNodeColor();
      while (color == prevColor) {
        color = nodeFuncs::randomiseNodeColor();
      }
      prevColor = color;

      FieldSingletone::instance()->at(index)->setColor(color);
      FieldSingletone::instance()->at(index)->setDestroyFlag(false);
      FieldSingletone::instance()->at(index)->setScale(0.1);

      SceneSingletone::instance()->addItem(FieldSingletone::instance()->at(index));

      QGraphicsItemAnimation * animation = new QGraphicsItemAnimation(parameters::replaceTimer);
      animation->setItem(FieldSingletone::instance()->at(index));
      animation->setScaleAt(1, 10, 10);
      animation->setTimeLine(parameters::replaceTimer);

      index++;
    }
  }
}

bool nodeFuncs::isDestroyable(size_t index, parameters::direction_t direction)
{
  if (index > parameters::FIELD_SIZE * parameters::FIELD_SIZE || direction == parameters::direction_t::NONE) {
    return false;
  }

  int verticalOffset = 0;
  int horizontalOffset = 0;

  const double deltaX = (*FieldSingletone::instance())[index]->pos().x() - (*FieldSingletone::instance())[index]->getHomePos().x();
  const double deltaY = (*FieldSingletone::instance())[index]->pos().y() - (*FieldSingletone::instance())[index]->getHomePos().y();

  size_t newIndex = index;

  if (direction == parameters::direction_t::VERTICAL) {
    horizontalOffset = parameters::FIELD_SIZE;

    if (deltaY < 0) {
      --newIndex;
      verticalOffset = -1;
    } else {
      ++newIndex;
      verticalOffset = 1;
    }

  } else if (direction == parameters::direction_t::HORIZONTAL) {
    verticalOffset = 1;

    if (deltaX < 0) {
      newIndex -= parameters::FIELD_SIZE;
      horizontalOffset = -parameters::FIELD_SIZE;
    } else {
      newIndex += parameters::FIELD_SIZE;
      horizontalOffset = parameters::FIELD_SIZE;
    }

  }

  const size_t column = newIndex / parameters::FIELD_SIZE;
  const size_t string = newIndex % parameters::FIELD_SIZE;

  if (direction == parameters::direction_t::VERTICAL) {

    switch (string) {
    case 0:
    case parameters::FIELD_SIZE - 1:
      {
        return isSpecialCaseDestroyable(index, newIndex, column, horizontalOffset)
            || isDefaultCaseDestroyable(newIndex, index, column, -verticalOffset, horizontalOffset) ;
      }
    case 1:
      {
        if (deltaY > 0) {
          return isDefaultCaseDestroyable(index, newIndex, column, verticalOffset, horizontalOffset)
              || isSpecialCaseDestroyable(newIndex, index, column, horizontalOffset);
        } else {
          return isSpecialCaseDestroyable(index, newIndex, column, horizontalOffset)
              || isDefaultCaseDestroyable(newIndex, index, column, -verticalOffset, horizontalOffset) ;
        }
      }
    case parameters::FIELD_SIZE - 2:
      {
        if (deltaY < 0) {
          return isDefaultCaseDestroyable(index, newIndex, column, verticalOffset, horizontalOffset)
              || isSpecialCaseDestroyable(newIndex, index, column, horizontalOffset);
        } else {
          return isSpecialCaseDestroyable(index, newIndex, column, horizontalOffset)
              || isDefaultCaseDestroyable(newIndex, index, column, -verticalOffset, horizontalOffset);
        }
      }
    default:
      {
        if ((deltaY > 0 && string == 2) || (deltaY < 0 && string == parameters::FIELD_SIZE - 3) ) {
          return isDefaultCaseDestroyable(index, newIndex, column, verticalOffset, horizontalOffset)
              || isSpecialCaseDestroyable(newIndex, index, column, horizontalOffset);
        }

        return isDefaultCaseDestroyable(index, newIndex, column, verticalOffset, horizontalOffset)
            || isDefaultCaseDestroyable(newIndex, index, column, -verticalOffset, horizontalOffset);
      }
    }

  } else {

    switch (column) {
    case 0:
    case parameters::FIELD_SIZE - 1:
      {
        return isSpecialCaseDestroyable(index, newIndex, string, verticalOffset)
            || isDefaultCaseDestroyable(newIndex, index, string, -horizontalOffset, verticalOffset) ;
      }
    case 1:
      {
        if (deltaX > 0) {
          return isDefaultCaseDestroyable(index, newIndex, string, horizontalOffset, verticalOffset)
              || isSpecialCaseDestroyable(newIndex, index, string, verticalOffset);
        } else {
          return isSpecialCaseDestroyable(index, newIndex, string, verticalOffset)
              || isDefaultCaseDestroyable(newIndex, index, string, -horizontalOffset, verticalOffset);
        }
      }
    case parameters::FIELD_SIZE - 2:
      {
        if (deltaX < 0) {
          return isDefaultCaseDestroyable(index, newIndex, string, horizontalOffset, verticalOffset)
              || isSpecialCaseDestroyable(newIndex, index, string, verticalOffset);
        } else {
          return isSpecialCaseDestroyable(index, newIndex, string, verticalOffset)
              || isDefaultCaseDestroyable(newIndex, index, string, -horizontalOffset, verticalOffset);
        }
      }
    default:
      {
        if ((deltaX > 0 && column == 2) || (deltaX < 0 && column == parameters::FIELD_SIZE - 3)) {
          return isDefaultCaseDestroyable(index, newIndex, string, horizontalOffset, verticalOffset)
              || isSpecialCaseDestroyable(newIndex, index, string, verticalOffset);
        }

        return isDefaultCaseDestroyable(index, newIndex, string, horizontalOffset, verticalOffset)
            || isDefaultCaseDestroyable(newIndex, index, string, -horizontalOffset, verticalOffset);
      }
    }

  }
}

bool nodeFuncs::isSideDestroyable(size_t index, size_t newIndex, int radius)
{
  return (FieldSingletone::instance()->at(newIndex + radius)->getColor()
      == FieldSingletone::instance()->at(newIndex + radius * 2)->getColor()
        && FieldSingletone::instance()->at(index)->getColor()
        == FieldSingletone::instance()->at(newIndex + radius)->getColor());
}

bool nodeFuncs::isNeighborsDestroyable(size_t index, size_t newIndex, int radius)
{
  return (FieldSingletone::instance()->at(newIndex + radius)->getColor() == FieldSingletone::instance()->at(index)->getColor()
        && FieldSingletone::instance()->at(index)->getColor() == FieldSingletone::instance()->at(newIndex - radius)->getColor());
}

bool nodeFuncs::isSpecialCaseDestroyable(size_t index, size_t newIndex, size_t coordPlace, int radius)
{
  switch (coordPlace) {
  case 0:
    {
      return isSideDestroyable(index, newIndex, radius);
    }
  case 1:
    {
      return (isNeighborsDestroyable(index, newIndex, radius) || isSideDestroyable(index, newIndex, radius));
    }
  case parameters::FIELD_SIZE - 2:
    {
      return (isNeighborsDestroyable(index, newIndex, radius) || isSideDestroyable(index, newIndex, -radius));
    }
  case parameters::FIELD_SIZE - 1:
    {
      return isSideDestroyable(index, newIndex, -radius);
    }
  default:
    {
      return (isSideDestroyable(index, newIndex, radius) || isSideDestroyable(index, newIndex, -radius)
              || isNeighborsDestroyable(index, newIndex, radius));
    }
  }
}

bool nodeFuncs::isDefaultCaseDestroyable(size_t index, size_t newIndex, size_t coordPlace, int mainRadius, int sideRadius)
{
  switch (coordPlace) {
  case 0:
    {
      return (isSideDestroyable(index, newIndex, sideRadius) || isSideDestroyable(index, newIndex, mainRadius));
    }
  case 1:
    {
      return (isNeighborsDestroyable(index, newIndex, sideRadius) || isSideDestroyable(index, newIndex, sideRadius)
              || isSideDestroyable(index, newIndex, mainRadius));
    }
  case parameters::FIELD_SIZE - 2:
    {
      return (isNeighborsDestroyable(index, newIndex, sideRadius) || isSideDestroyable(index, newIndex, -sideRadius)
              || isSideDestroyable(index, newIndex, mainRadius));
    }
  case parameters::FIELD_SIZE - 1:
    {
      return (isSideDestroyable(index, newIndex, -sideRadius) || isSideDestroyable(index, newIndex, mainRadius));
    }
  default:
    {
      return (isSideDestroyable(index, newIndex, sideRadius) || isSideDestroyable(index, newIndex, -sideRadius)
              || isNeighborsDestroyable(index, newIndex, sideRadius) || isSideDestroyable(index, newIndex, mainRadius));
    }
  }
}
