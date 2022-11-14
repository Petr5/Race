#include "window.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
Window::Window()
{
    width_road = 60;
    setCar();
    setField();
    initialize_constant_of_moving();
    QTimer* move_the_car = new QTimer();
   connect(move_the_car, SIGNAL(timeout()), this, SLOT(moveCar()));

//    slotSendToServer();
    move_the_car->start(5000);
}
void Window::initialize_constant_of_moving(){
    x_step = 1;
    y_step = 1;
    x_skew_step = 0.7071;
    y_skew_step = 0.7071;
}
void Window::setCar()
{
    QPainterPath car;
    car.addRoundedRect(0, 0,  2 * width_road / 3,  width_road / 3, 3, 3);
    this->car = car;
    update();
}

void Window::setField()
{
    QPainterPath roundRectPath;
    center_y_window = size().height() / 2;
    center_x_window = size().width() / 2;
//    qInfo() << "try to set field";
    roundRectPath.addRoundedRect(0,0, 2 * center_x_window, 2 * center_y_window, 30, 30);
    roundRectPath.addRoundedRect(width_road, width_road, 2 * center_x_window - 2 * width_road, 2 * center_y_window - 2 * width_road,30,30);
    this->field = roundRectPath;
    update();
}

void Window::drawCar(QPainter &painter)
{
    painter.resetTransform();
    painter.translate(width_road / 3 + x, width_road / 3 + y);
    painter.fillPath(car, Qt::blue);

}
void Window::drawField(QPainter &painter)
{
    setField();
    painter.resetTransform();

    painter.drawPath(field);
}
void Window::paintEvent(QPaintEvent *event)
{
//    qInfo() << "window size now is " << size();
//    qInfo() << "height is " << size().height();

    QPainter painter(this);
    my_painter  = &painter;
//    qDebug() << my_painter;
    painter.setRenderHint(QPainter::Antialiasing);
//    painter.fillRect(event->rect(), QBrush(Qt::white));
    painter.save();
    drawField(painter);
    drawCar(painter);
    painter.restore();

}


void Window::keyPressEvent(QKeyEvent *event)
{
//    qDebug("At least came here");

    QString text = event->text();
    pressed_buttons.append(text[0]);
    qDebug() << "press " + text;
    moveCar();
}


void Window::keyReleaseEvent(QKeyEvent *event)
{
//    qDebug("At least came here");

    QString text = event->text();
    int i = 0;
    while (i < pressed_buttons.size()){
        if (pressed_buttons[i] == text[0]) pressed_buttons.erase(pressed_buttons.begin() + i);
        else ++i;
    }
//    pressed_buttons.erase(pressed_buttons.begin() + pressed_buttons.at());
//    forward = event->text().toStdString().at(0);
    qDebug() << "release " + text;
//    moveCar();
}

void Window::define_the_direction_ofmoving(){
    if (pressed_buttons.size() > 2){
        qDebug() << "press only on two or one buttons to correctly move";
    }
    else{
//        QChar fButton = pressed_buttons[0];
//        QChar sButton = pressed_buttons[1];
        QString signal = QString(pressed_buttons[0]) + pressed_buttons[1];
        direction = 1000; // not goood!!
        if (signal == "wa" or signal == "aw"){
            direction = 0;
        }
        else if (signal == "ws" or signal == "sw"){
            direction = 2;
        }
        else if (signal == "w"){
            direction = 1;
        }
        else if (signal == "as" or signal == "sa"){
            direction = 3;
        }
        else if (signal == "ds" or signal == "sd"){
            direction = 5;
        }
        else if(signal == "s"){
            direction = 4;
        }
        else{
            qDebug() << "error: "  << "something went wrong" << "direction no in set from 0 to 5";
        }
    }
}
void Window::moveCar(){
    define_the_direction_ofmoving();
    switch(direction){
    case 0:
        x += x_skew_step;
        y -= y_skew_step;
        break;
    case 1:
        x += x_step;
        break;
    case 2:
        x += x_skew_step;
        y += y_skew_step;
        break;
    case 3:
        x -= x_skew_step;
        y -= y_skew_step;
        break;
    case 4:
        x -= x_step;
        break;
    case 5:
        x -= x_skew_step;
        y += y_skew_step;
        break;
    default:
        ;
    }

    update();
}
