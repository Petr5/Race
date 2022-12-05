#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPainterPath>
class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

public slots:

private slots:
    void moveCar();
private:


    QPainterPath car;
    QPainterPath field;
    int width_road;
    int direction;

    double speed;
    double step_speed;
    double deceleration;
    double alpha;
    double alpha_step;
    double alpha_step_step;
    double alpha_step_step_scale;

    bool vertical_collision = false;
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
