#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(screen);
    scene->addRect(0, 0, 100, 100, QPen(Qt::black), QBrush(Qt::green));
}

MainWindow::~MainWindow()
{
    delete ui;
}
QGraphicsScene* scene;

