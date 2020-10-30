#include "mainwindow.hpp"

#include <QStyleFactory>
#include <QFileDialog>
#include <QDesktopServices>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->PlayButton->setStyle(QStyleFactory::create("Fusion"));
  ui->InfoButton->setStyle(QStyleFactory::create("Fusion"));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_InfoButton_clicked()
{
  QDesktopServices::openUrl(QUrl("https://hastebin.com/korirofiku.bash", QUrl::TolerantMode));
}

void MainWindow::on_PlayButton_clicked()
{
  playWindow_ = new PlayWindow();
  playWindow_->show();
  this->close();
}
