#ifndef WINDOW_H
#define WINDOW_H
#include "client.h"
#include <QWidget>
#include <QPainterPath>
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(Client * client);

public slots:

private slots:
    void moveCar();
public slots:
    void updateInformationForServer();
private:
    Client * client;
    QTimer* timer;
    QPainterPath car;
    QPainterPath field;
    int width_road;
    int direction;

    double speed;
    double alpha;
    double alpha_step;
    QList<QChar> pressed_buttons;
    void setCar();
    void drawCar(QPainter &painter);
    void paintEvent(QPaintEvent *event);

    void setField();
    void drawField(QPainter &painter);
    void define_the_direction_ofmoving();
    void initialize_constant_of_moving();
protected:
    void keyPressEvent(QKeyEvent *event);
    double x = 0;
    double y = 0;
    int center_x_window;
    int center_y_window;
    char forward = ' ';
    QPainter* my_painter = nullptr;
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // WINDOW_H
