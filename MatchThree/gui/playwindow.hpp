#ifndef PLAYWINDOW_HPP
#define PLAYWINDOW_HPP

#include <QGraphicsView>
#include <QPushButton>
#include <unordered_map>

class PlayWindow : public QGraphicsView {
  Q_OBJECT
public:
  explicit PlayWindow(QWidget * parent = nullptr);
  ~PlayWindow();

private slots:
  void slotTimerAlarm();
  void onRestartButtonClicked();
  void onQuitButtonClicked();

private:
  std::unordered_map< std::string, QGraphicsSimpleTextItem * > textItems_;
  std::unordered_map< std::string, QPushButton * > buttons_;
  QTimer * timer_;
  size_t remainimgTimeMSec_;
};

#endif
