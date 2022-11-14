#ifndef WINDOW_H
#define WINDOW_H

#include "client.h"
#include <QWidget>
#include <QPainterPath>
#include <QTimer>
class Client;
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(Client * client);
    friend Client;
    int x = 66;
    int y = 66;

public slots:
    void updateInformationForServer();
private:
    QPainterPath shape;
    QTimer *timer;
    Client* client;
    void setShape(const QPainterPath &shape);
    void setShape();
    void drawShape(QPainter &painter);
    void paintEvent(QPaintEvent *event);
    void moveCar();

    void setField();
protected:
    void keyPressEvent(QKeyEvent *event);

    char forward = ' ';
    QPainter* my_painter = nullptr;
private slots:
    void print_coordinates();
};

#endif // WINDOW_H
