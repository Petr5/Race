#include "window.h"

#include "window.h"
#include "client.h"
#include <QPainter>
#include <QKeyEvent>
#include <QSignalMapper>
Window::Window(Client * client)
{
    this->client = client;
    setShape();
    timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(updateInformationForServer()));
   connect(timer, SIGNAL(timeout()), this, SLOT(updateInformationForServer()));

//    slotSendToServer();
    timer->start(5000);
}


void Window::updateInformationForServer(){
    QSignalMapper* signalMapper = new QSignalMapper (this) ;
    connect (timer,  SIGNAL(timeout()), signalMapper, SLOT(map())) ;
    connect (timer,  SIGNAL(timeout()), this, SLOT(print_coordinates()));
    signalMapper -> setMapping (timer, QString("300 x=" + QString::number(x) + " y=" + QString::number(y))) ;
    connect(signalMapper, SIGNAL(mappedString(QString)), client, SLOT(slotSendToServer(QString)));
}

void Window::setShape()
{

    QPainterPath truck;
    truck.setFillRule(Qt::WindingFill);
    truck.moveTo(0.0, 87.0);
    truck.lineTo(0.0, 60.0);
    truck.lineTo(10.0, 60.0);
    truck.lineTo(35.0, 35.0);
    truck.lineTo(100.0, 35.0);
    truck.lineTo(100.0, 87.0);
    truck.lineTo(0.0, 87.0);
    truck.moveTo(17.0, 60.0);
    truck.lineTo(55.0, 60.0);
    truck.lineTo(55.0, 40.0);
    truck.lineTo(37.0, 40.0);
    truck.lineTo(17.0, 60.0);
    truck.addEllipse(17.0, 75.0, 25.0, 25.0);
    truck.addEllipse(63.0, 75.0, 25.0, 25.0);

    this->shape = truck;
    update();
}

void Window::setField()
{
     my_painter->translate(50.0, 50.0);
    QPainterPath roundRectPath;
       roundRectPath.moveTo(80.0, 35.0);
       roundRectPath.arcTo(70.0, 30.0, 10.0, 10.0, 0.0, 90.0);
       roundRectPath.lineTo(25.0, 30.0);
       roundRectPath.arcTo(20.0, 30.0, 10.0, 10.0, 90.0, 90.0);
       roundRectPath.lineTo(20.0, 65.0);
       roundRectPath.arcTo(20.0, 60.0, 10.0, 10.0, 180.0, 90.0);
       roundRectPath.lineTo(75.0, 70.0);
       roundRectPath.arcTo(70.0, 60.0, 10.0, 10.0, 270.0, 90.0);
       roundRectPath.closeSubpath();
       my_painter->setPen(Qt::red);
       my_painter->drawPath(roundRectPath);
    update();
}

void Window::print_coordinates(){
    qInfo() << x << " " << y;
}
void Window::drawShape(QPainter &painter)
{
    painter.fillPath(shape, Qt::blue);
}
void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    my_painter  = &painter;
    qDebug() << my_painter;
    painter.setRenderHint(QPainter::Antialiasing);
//    painter.fillRect(event->rect(), QBrush(Qt::white));
    painter.translate(x, y);

    painter.save();
    drawShape(painter);
    painter.restore();

}


void Window::keyPressEvent(QKeyEvent *event)
{
    qDebug("At least came here");
//    my_painter->rotate(10);
    qInfo() << "now real cordinates is " << x << y;
    QString text = event->text();
    forward = event->text().toStdString().at(0);
    qDebug() << text;
    moveCar();
}

void Window::moveCar(){
    switch (forward) {
    case 'w':
        x += 5;
        break;
    case 'a':
        y += 5;
        break;
    case 's':
        x -= 5;
        break;
    case 'd':
        y -= 5;
    default:
        ;
    }
    update();
}


