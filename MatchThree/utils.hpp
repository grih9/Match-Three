#ifndef UTILITS_HPP
#define UTILITS_HPP

#include <iostream>
#include <QPainter>
#include <QTimeLine>

namespace parameters {
  enum class direction_t {
    VERTICAL,
    HORIZONTAL,
    NONE
  };

  enum class color_t {
    RED = Qt::red,
    GREEN = Qt::green,
    BLUE = Qt::blue,
    YELLOW = Qt::yellow,
    MAGENTA = Qt::magenta
  };

  static const size_t FIELD_SIZE = 8;
  static const size_t INTERVAL = 20;
  static const size_t DIAMETER = 50;
  static const size_t ANIMATION_PERIOD_MS = 200;
  static const size_t SLEEP_TIME = 300;
  static const size_t START_TIME = 60000;
  static const size_t ALMOST_END_TIME = 10000;
  static const size_t MSEC_IN_SEC = 1000;

  extern QTimeLine * timer;
  extern QTimeLine * replaceTimer;
  extern bool isDestroyable;
  extern bool isAnimationGoing;
  extern size_t score;
}

class Node;

namespace nodeFuncs {
  void add(Node *);
  void swap(size_t index1, size_t index2);
  void process();

  bool getDestroyableFlag();
  bool getAnimationFlag();
  void setDestroyableFlag(bool);
  void setAnimationFlag(bool);
  void setDestroyFlags(parameters::direction_t, size_t);

  void addToScore(size_t);
  size_t getScore();
  void resetScore();

  Qt::GlobalColor randomiseNodeColor();
  void generateNewNodes();

  bool isDestroyable(size_t, parameters::direction_t);
  bool isSideDestroyable(size_t index, size_t newIndex, int);
  bool isNeighborsDestroyable(size_t index, size_t newIndex, int);
  bool isSpecialCaseDestroyable(size_t index, size_t newIndex, size_t coordPlace, int radius);
  bool isDefaultCaseDestroyable(size_t index, size_t newIndex, size_t coordPlace, int mainRadius, int sideRadius);
};

#endif
