#include "scene-singletone.hpp"

static QGraphicsScene * scene_;

QGraphicsScene * SceneSingletone::instance()
{
  if (!scene_) {
    scene_ = new QGraphicsScene();
  }

  return scene_;
}
