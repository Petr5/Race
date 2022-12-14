#ifndef WINDOW_H
#define WINDOW_H
#include "client.h"
#include "qpainter.h"
#include <QWidget>
#include <QPainterPath>
#include <QDateTime>
#include <QSoundEffect>
//#include <QMatrix>
class Window : public QWidget
{
    Q_OBJECT

public:
    friend class Client;
    Client * client;
    Window(Client * client);

    void CallDrawCars(QString message);
    QString NickName;

    void drawCar(QPainter &painter, double x, double y, double alpha);
    QDateTime time_of_start_game;
public slots:

private slots:
    void moveCar();
    void synchronizeClientWithServer();
public slots:
    void updateInformationForServer();
    void PrintCurrentrating();
    void checkControlPoints();
private:
    QSoundEffect* effect;

    QTimer* timer;
    QPainterPath car;
    QPainterPath field;
    double width_road;
    int direction;

    double speed;
    double standard_step_speed;
    double step_speed;
    double standard_max_speed;
    double max_speed;
    double standard_declaration;
    double deceleration;
    double alpha;
    double alpha_step;
    double alpha_step_step;
    double alpha_step_step_scale;

    bool collision = false;
    QList<int> blocked_direction;
    QDateTime time_of_colision  = QDateTime::currentDateTime();;
    bool shift_pressed = false;
    bool space_pressed = false;
    QList<QChar> pressed_buttons;
    QMap<QString, QList<double>> coordinates_of_players;
    QMap<QString, QList<double>> time_of_visiting_control_points;

    int nmb_of_contral_points;
    QList<QPainterPath> control_points;
    QMap<int, QPainterPath> map_id_to_path;
    QMap<int, QList<qint64>> id_point_to_his_timestamps;
    QList<int> id_to_numbs_visit_point = {1,1,1,1};


    QList<qint64> time_of_intersection_control_points;

    void setCar();
    void drawCars(QPainter &painter);
    void paintEvent(QPaintEvent *event);

    void setField();
    void drawField(QPainter &painter);
    void define_the_direction_ofmoving();
    void initialize_constant_of_moving();

    void define_the_acceleration_of_speed();
    void change_acceleration_of_speed();
    void shift_speed_up();
    void space_speed_down();
    void drawControlPoint(QPainter &painter, QPainterPath path);
    void setControlPoints(QList<QList<double> > control_points);
    void drawControlPoints(QPainter &painter);


    void drawStatistic(QPainter &painter);
    void PrintStatistics(QPainter &painter);
    void fill_road(QPainter &painter);
    void CallPrintStatistics(QString message);
    void setSoundEffect();
protected:
    void keyPressEvent(QKeyEvent *event);
    double x = 0;
    double y = 0;
    double x_server;
    double y_server;
    int center_x_window = -1;
    int center_y_window = -1;
    char forward = ' ';
    QPainter* my_painter = nullptr;
    QPainter painter;
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // WINDOW_H
