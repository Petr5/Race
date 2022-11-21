#ifndef WINDOW_H
#define WINDOW_H
#include "client.h"
#include "qpainter.h"
#include <QWidget>
#include <QPainterPath>
class Window : public QWidget
{
    Q_OBJECT

public:
    friend class Client;
    Client * client;
    Window(Client * client);

    void CallDrawCars(QString message);
    QString NickName;

    void drawCar(QPainter &painter);
public slots:

private slots:
    void moveCar();
    void synchronizeClientWithServer();
public slots:
    void updateInformationForServer();
private:

    QTimer* timer;
    QPainterPath car;
    QPainterPath field;
    int width_road;
    int direction;

    double speed;
    double alpha;
    double alpha_step;
    double alpha_step_step;
    double alpha_step_step_scale;

    QList<QChar> pressed_buttons;
    QMap<QString, QList<double>> coordinates_of_players;

    void setCar();
    void drawCars(QPainter &painter);
    void paintEvent(QPaintEvent *event);

    void setField();
    void drawField(QPainter &painter);
    void define_the_direction_ofmoving();
    void initialize_constant_of_moving();

protected:
    void keyPressEvent(QKeyEvent *event);
    double x = 0;
    double y = 0;
    double x_server;
    double y_server;
    int center_x_window;
    int center_y_window;
    char forward = ' ';
    QPainter* my_painter = nullptr;
    QPainter painter;
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // WINDOW_H
