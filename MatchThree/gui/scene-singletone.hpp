#ifndef SCENESINGLETONE_HPP
#define SCENESINGLETONE_HPP

#include <QGraphicsScene>

class Node;

class SceneSingletone {
public:
  static QGraphicsScene * instance();
};

#endif
