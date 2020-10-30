#ifndef NODE_HPP
#define NODE_HPP

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsItemAnimation>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>

#include "utils.hpp"

class Node : public QObject, public QGraphicsItem
{
  Q_OBJECT
public:
  Node(Qt::GlobalColor color, size_t index, size_t diameter);

  void moveToPos(const QPointF &);
  void initAnimation();
  void initReplaceAnimation();

  void setColor(const Qt::GlobalColor color);
  void setHomePos(const QPointF &);
  void setIndex(size_t index);
  void setDestroyFlag(bool flag);

  Qt::GlobalColor getColor() const;
  QPointF getHomePos() const;
  size_t getIndex() const;
  bool getDestroyFlag() const;

protected Q_SLOTS:
  void onAnimationFinished();
  void onReplaceAnimationFinished();

private:
  QGraphicsItemAnimation * animation_;
  parameters::direction_t movingDir_;
  QPointF homePos_;
  Qt::GlobalColor color_;
  size_t index_;
  size_t diameter_;
  bool isDestroyable_;

  QRectF boundingRect() const;
  QPainterPath shape() const;

  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *);
  void mousePressEvent(QGraphicsSceneMouseEvent *);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
};

#endif
