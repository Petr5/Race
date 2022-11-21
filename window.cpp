#include "window.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QSignalMapper>
Window::Window(Client * client)

{
    this->client = client;
    NickName = client->NickName;
    client->setPointer_to_UI(this);
    width_road = 60;
    setCar();
    setField();
    initialize_constant_of_moving();
    QTimer* move_the_car = new QTimer();
   connect(move_the_car, SIGNAL(timeout()), this, SLOT(moveCar()));

    move_the_car->start(50);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateInformationForServer()));
    timer->start(50);


//    QTimer* synchronize = new QTimer();
//    connect(synchronize, SIGNAL(timeout()), this, SLOT(synchronizeClientWithServer()));
//    synchronize->start(5000);
}
static int nmb_packet = -1;
void Window::updateInformationForServer(){
//    QSignalMapper* signalMapper_300 = new QSignalMapper (this) ;
//    connect (timer,  SIGNAL(timeout()), signalMapper_300, SLOT(map())) ;
//    qInfo() << "start update information on server";
//    qInfo() << "sent to server " << QString("300 " + QString::number(x) + " " + QString::number(y));
//    signalMapper_300 -> setMapping (timer, QString("300 " + QString::number(x) + " " + QString::number(y))) ;
//    connect(signalMapper_300, SIGNAL(mappedString(QString)), client, SLOT(slotSendToServer(QString)));
    ++nmb_packet;
    qInfo() << "sent to server " + QString("300 " + QString::number(x) + " " + QString::number(y) + " " + QString::number(alpha)) + "packet " + QString::number(nmb_packet);
    client->SendToServer(QString("300 " + QString::number(x) + " " + QString::number(y) + " " + QString::number(alpha)) + " packet " + QString::number(nmb_packet));

//    QSignalMapper* signalMapper_200 = new QSignalMapper (this) ;
//    connect (timer,  SIGNAL(timeout()), signalMapper_200, SLOT(map())) ;
//    signalMapper_200 -> setMapping (timer, QString("200 ") + "packet " + QString::number(nmb_packet)) ;
//    connect(signalMapper_200, SIGNAL(mappedString(QString)), client, SLOT(slotSendToServer(QString)));
    client->SendToServer(QString("200 ") + "packet " + QString::number(nmb_packet));
}
void Client::CallUpdateUI(QString message){
//    QSignalMapper* signalMapper_211 = new QSignalMapper (this) ;
//    connect (timer,  SIGNAL(timeout()), signalMapper_200, SLOT(map())) ;
//    signalMapper_211 -> setMapping (timer, QString("200")) ;
//    connect(signalMapper_211, SIGNAL(mappedString(QString)), client, SLOT(slotSendToServer(QString)));/
    //qInfo () << "mainwindow" << window;

    window->CallDrawCars(message);
}
void Window::initialize_constant_of_moving(){
    speed = 0.5;
    alpha = 0;
    alpha_step = 0;
    alpha_step_step = 0;
    alpha_step_step_scale = 0.3;
}
void Window::CallDrawCars(QString message){
    //qInfo() << "callDrawCars message is " + message;
    int amount = message.split('\n')[0].split(" ")[1].toInt();
    qInfo() << "amount of cars is " << amount;
    qInfo() << "split message is " << message.split('\n');
    for (int i = 1; i <= amount; ++i){
        qInfo() << "i is " << i;
//        coordinates_of_players[message.split('\n')[i].split(" ")[0]] = qMakePair(message.split('\n')[i].split(" ")[1].toDouble(), message.split('\n')[i].split(" ")[2].toDouble());
        const QList<double> list = QList<double>({message.split('\n')[i].split(" ")[1].toDouble(), message.split('\n')[i].split(" ")[2].toDouble(), message.split('\n')[i].split(" ")[3].toDouble()});
        qInfo() << "list is " << list;
        coordinates_of_players.insert(message.split('\n')[i].split(" ")[0], list);
    }
    qInfo() << "map on client is " << coordinates_of_players;
    update();
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
//    //qInfo() << "try to set field";
    roundRectPath.addRoundedRect(0,0, 2 * center_x_window, 2 * center_y_window, 30, 30);
    roundRectPath.addRoundedRect(width_road, width_road, 2 * center_x_window - 2 * width_road, 2 * center_y_window - 2 * width_road,30,30);
    this->field = roundRectPath;
    update();
}

void Window::drawCars(QPainter &painter)
{
    qInfo() << "size of map of clients on client " << coordinates_of_players.size();
    qInfo() << "map is " << coordinates_of_players;
    for (auto it = coordinates_of_players.begin(); it != coordinates_of_players.end(); ++it){
        double x = it.value()[0];
        double y = it.value()[1];
        double alpha = it.value()[2];
        qInfo() << "values frmo the server are x " << x << " y " << y << " z " << alpha;
        qInfo() << "nickname on the server is " << it.key() << " while on the client " << NickName;
        if(it.key() == NickName){
            qInfo() << "nickname on the server and client IS THE SAME!!!!!!!!!!!!!!!!!!!!!";
            x_server = it.value()[0];
            y_server = it.value()[1];
            this->alpha = alpha;
        }

//        qInfo() << it.key() << " x y from server " << x << y << " while client x y are " << this->x << " " << this->y;

        painter.resetTransform();
        painter.translate(width_road / 3 + x, width_road / 3 + y);
        painter.rotate(-alpha);
        painter.fillPath(car, Qt::blue);
    }

}
void Window::synchronizeClientWithServer(){
    x = x_server;
    y = y_server;
}

void Window::drawCar(QPainter &painter)
{
    painter.resetTransform();
    painter.translate(width_road / 3 + this->x, width_road / 3 + this->y);
    painter.rotate(-alpha);
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
        qInfo() << "paintevent starts";
//    //qInfo() << "window size now is " << size();
//    //qInfo() << "height is " << size().height();

    QPainter painter(this);
    my_painter  = &painter;
//    qDebug() << my_painter;
    painter.setRenderHint(QPainter::Antialiasing);
//    painter.fillRect(event->rect(), QBrush(Qt::white));
    painter.save();
    drawField(painter);
    drawCars(painter);
    painter.restore();

}


void Window::keyPressEvent(QKeyEvent *event)
{
    qDebug("At least came here");

    QString text = event->text();
    pressed_buttons.append(text[0]);
    qDebug() << "press " + text;
     qDebug("At least came here");
}

void Window::keyReleaseEvent(QKeyEvent *event)
{
    qDebug("At least came here 2");

    QString text = event->text();
    int i = 0;
    while (i < pressed_buttons.size()){
        if (pressed_buttons[i] == text[0]) pressed_buttons.erase(pressed_buttons.begin() + i);
        else ++i;
    }

    qDebug() << "release " + text;
}

void Window::define_the_direction_ofmoving(){
    //qInfo() << "try to determine current direction " << pressed_buttons;
    if (pressed_buttons.size() > 2){
        qDebug() << "press only on two or one buttons to correctly move";
    }
    else if(pressed_buttons.size() == 2){
//        QChar fButton = pressed_buttons[0];
//        QChar sButton = pressed_buttons[1];

        QString signal = QString(pressed_buttons[0]) + pressed_buttons[1];
        direction = 1000; // not goood!!
        if (signal == "wa" or signal == "aw"){
            direction = 0;
        }
        else if (signal == "wd" or signal == "dw"){
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
    else if (pressed_buttons.size() == 1)
    {
        QString signal = QString(pressed_buttons[0]);
        direction = 1; // not goood!!
        if (signal == "w"){
            direction = 1;
        }
        else if(signal == "s"){
            direction = 4;
        }
        else if (signal == "a"){
            direction = 0;
        }
        else if (signal == "d"){
            direction = 2;
        }
        else{
            qDebug() << "error: "  << "something went wrong" << "direction no in set from 0 to 5";
        }
    }
    else{
        direction = 1;
    }
}
void Window::moveCar(){
//    qDebug() << "try to move car" << "current direction is "<< direction;
    define_the_direction_ofmoving();
    switch(direction){
    case 0:
            x +=  qCos(qDegreesToRadians(alpha)) * speed;
            y -= qSin(qDegreesToRadians(alpha)) * speed;
            alpha += alpha_step;
            alpha_step += alpha_step_step;
            alpha_step_step = alpha_step_step_scale;
            break;
        case 1:
            x +=  qCos(qDegreesToRadians(alpha)) * speed;
            y -= qSin(qDegreesToRadians(alpha)) * speed;
            alpha += alpha_step;

            break;
        case 2:
            x +=  qCos(qDegreesToRadians(alpha)) * speed;
            y -= qSin(qDegreesToRadians(alpha)) * speed;
            alpha += alpha_step;
//            if ()
            alpha_step += alpha_step_step;
            alpha_step_step = -alpha_step_step_scale;
            break;
        case 3:
            x -=  qCos(qDegreesToRadians(alpha)) * speed;
            y += qSin(qDegreesToRadians(alpha)) * speed;
            alpha -= alpha_step;
            break;
        case 4:
            x -=  qCos(qDegreesToRadians(alpha)) * speed;
            y += qSin(qDegreesToRadians(alpha)) * speed;
            break;
        case 5:
            x -=  qCos(qDegreesToRadians(alpha)) * speed;
            y += qSin(qDegreesToRadians(alpha)) * speed;
            alpha += alpha_step;
            break;
        default:
            ;
    }

    update();
}
