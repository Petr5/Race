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

    ++nmb_packet;
    //qInfo() << "sent to server " + QString("300 " + QString::number(x) + " " + QString::number(y) + " " + QString::number(alpha)) + "packet " + QString::number(nmb_packet);
    client->SendToServer(QString("300 " + QString::number(x) + " " + QString::number(y) + " " + QString::number(alpha)) + " packet " + QString::number(nmb_packet));
    client->SendToServer(QString("200 ") + "packet " + QString::number(nmb_packet));
}
void Client::CallUpdateUI(QString message){

    window->CallDrawCars(message);
}
void Client::start_the_game(){
//   window = new Window(this);
    window->show();
}
void Window::initialize_constant_of_moving(){
    speed = 0.5;
    standard_step_speed = 0.05 * speed;
    step_speed = standard_step_speed;
    standard_max_speed = 3;
    max_speed = standard_max_speed;
    standard_declaration = 0.2 * standard_step_speed;
    deceleration = standard_declaration;
    alpha = 0;
    alpha_step = 0;
    alpha_step_step = 0;
    alpha_step_step_scale = 0.3;

}
void Window::CallDrawCars(QString message){
    ////qInfo() << "callDrawCars message is " + message;
    int amount = message.split('\n')[0].split(" ")[1].toInt();
    //qInfo() << "amount of cars is " << amount;
    //qInfo() << "split message is " << message.split('\n');
    for (int i = 1; i <= amount; ++i){
        //qInfo() << "i is " << i;
//        coordinates_of_players[message.split('\n')[i].split(" ")[0]] = qMakePair(message.split('\n')[i].split(" ")[1].toDouble(), message.split('\n')[i].split(" ")[2].toDouble());
        const QList<double> list = QList<double>({message.split('\n')[i].split(" ")[1].toDouble(), message.split('\n')[i].split(" ")[2].toDouble(), message.split('\n')[i].split(" ")[3].toDouble()});
        //qInfo() << "list is " << list;
        coordinates_of_players.insert(message.split('\n')[i].split(" ")[0], list);
    }
    //qInfo() << "map on client is " << coordinates_of_players;
    update();
}
void Window::setCar()
{
    this->car = *new QPainterPath();
//    QRect rect = QRect(x + 20, y + 20,  40,  20);
    QRect rect = QRect(0, 0,  40,  20);

    car.addRect(rect);

     QTransform* t = new QTransform;
    t->translate(width_road / 3 + x, width_road / 3 + y).rotate( -alpha );
    car = t->map(car);
}

void Window::setField()
{
    QPainterPath roundRectPath;
    center_y_window = size().height() / 2;
    center_x_window = size().width() / 2;
//    ////qInfo() << "try to set field";
    roundRectPath.addRoundedRect(0,0, 2 * center_x_window, 2 * center_y_window, 30, 30);
    roundRectPath.addRoundedRect(width_road * 3, width_road * 3, 2 * center_x_window - 6 * width_road, 2 * center_y_window - 6 * width_road,30,30);
    this->field = roundRectPath;
    update();
}

void Window::drawCars(QPainter &painter)
{
//    //qInfo() << "size of map of clients on client " << coordinates_of_players.size();
//    //qInfo() << "map is " << coordinates_of_players;
    for (auto it = coordinates_of_players.begin(); it != coordinates_of_players.end(); ++it){
        double x = it.value()[0];
        double y = it.value()[1];
        double alpha = it.value()[2];
//        //qInfo() << "values frmo the server are x " << x << " y " << y << " z " << alpha;
//        //qInfo() << "nickname on the server is " << it.key() << " while on the client " << NickName;
        if(it.key() == NickName){
//            //qInfo() << "nickname on the server and client IS THE SAME!!!!!!!!!!!!!!!!!!!!!";
//            x_server = it.value()[0];
//            y_server = it.value()[1];
//            this->alpha = alpha;

//            painter.resetTransform();
//            painter.translate(width_road / 3 + x, width_road / 3 + y);
//            painter.rotate(-alpha);
//            if (collision) painter.fillPath(car, Qt::red);
//            else painter.fillPath(car, Qt::blue);
              drawCar(painter, x, y, alpha);
        }
        else{
//            painter.resetTransform();
//            painter.translate(width_road / 3 + x, width_road / 3 + y);
//            painter.rotate(-alpha);
//            painter.fillPath(car, Qt::blue);
            drawCar(painter, x, y, alpha);
        }
//        //qInfo() << it.key() << " x y from server " << x << y << " while client x y are " << this->x << " " << this->y;


    }

}
void Window::synchronizeClientWithServer(){
    x = x_server;
    y = y_server;
}

void Window::drawCar(QPainter &painter, double x, double y, double alpha)
{
    setCar();
//    painter.resetTransform();
//    QTransform* t = new QTransform;
//    t->translate(x, y).rotate( -alpha ).translate(-x, -y);
//    car = t->map(car);
    painter.drawPath(car);

    painter.resetTransform();
    painter.translate(width_road / 3 + x, width_road / 3 + y);
    painter.rotate(-alpha);
    QImage image ("/home/peter/Race/car2.png");
    image = image.scaled(40,20);
//    QMatrix rot;
//    rot.rotate(90);
//    QImage out = image.transformed(rot);
//    image.tra
    painter.drawImage(0, 0, image);
//    painter.drawPath(car);
//    painter.fillPath(car, Qt::blue);


}
void Window::drawField(QPainter &painter)
{
    setField();
    painter.resetTransform();

    painter.drawPath(field);
}
void Window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

//    double width = 40;
//    double height = 20;
//    double diagonal = qSqrt(width * width + height * height);
//    painter.drawPoint(QPoint(x, y));
//    painter.drawPoint(QPoint(x + diagonal * qCos(qDegreesToRadians(alpha) + qAtan2(height, width)), x + diagonal * qSin(qDegreesToRadians(alpha) + qAtan2(height, width)) ));
//    painter.drawPoint(QPoint(x + height * qCos(qDegreesToRadians(alpha + 90)), x + height * qSin(qDegreesToRadians(alpha + 90))));
//    painter.drawPoint(QPoint(x + width * qCos(qDegreesToRadians(alpha)), y + qSin(qDegreesToRadians(alpha))));
//    painter.drawPoint()
//    if (!field.contains(QPoint(x, y)) || !field.contains(QPoint(x + height * qCos(qDegreesToRadians(alpha + 90)), x + height * qSin(qDegreesToRadians(alpha + 90))))){
//        blocked_direction = {3, 4, 5};

//        qInfo() << "blocked direction are " << blocked_direction;
//    }
//    else if (!field.contains(QPoint(x + width * qCos(qDegreesToRadians(alpha)), y + qSin(qDegreesToRadians(alpha)))) || !field.contains(QPoint(x + diagonal * qCos(qDegreesToRadians(alpha) + qAtan2(height, width)), x + diagonal * qSin(qDegreesToRadians(alpha) + qAtan2(height, width)) ))){
//        blocked_direction = {0, 1, 2};
//        qInfo() << "blocked direction are " << blocked_direction;
//    }
//    else{
//        blocked_direction.clear();
//    }
//        qInfo() << "now " << "topRight  " << QPoint(x + diagonal * qCos(qDegreesToRadians(alpha) + qAtan2(height, width)), x + diagonal * qSin(qDegreesToRadians(alpha) + qAtan2(height, width)) ) << " topLeft contains " << field.contains(rotatedRect.topLeft()) << "bottom right contains " << field.contains(rotatedRect.bottomRight()) ;

//    qInfo() << "now " << "topRight contains " << field.contains(QPoint(x + diagonal * qCos(qDegreesToRadians(alpha) + qAtan2(height, width)), x + diagonal * qSin(qDegreesToRadians(alpha) + qAtan2(height, width)) )) \
//            << " topLeft contains " << field.contains(QPoint(x + width * qCos(qDegreesToRadians(alpha)), y + qSin(qDegreesToRadians(alpha))))
//            << "bottom right contains " << field.contains(QPoint(x + height * qCos(qDegreesToRadians(alpha + 90)), x + height * qSin(qDegreesToRadians(alpha + 90)))) ;
//    field.intersected()

    if (!collision){
        qInfo() << "after last collision " << time_of_colision.msecsTo(QDateTime::currentDateTime()) ;
        if (field.intersects(car) && !field.contains(car) && time_of_colision.msecsTo(QDateTime::currentDateTime()) > 5000){
            collision = true;
            speed = 0;
            alpha_step = 0;
            time_of_colision = QDateTime::currentDateTime();
            if (direction == 0 || direction == 1 || direction == 2  ){
                blocked_direction = {0,1,2};
            }

            else blocked_direction = {3,4,5};
        }
        else {
            collision = false;
            blocked_direction.clear();
        }

    }
    else{
        if(!blocked_direction.contains(direction)) collision = false;

    }


//    painter.setRenderHint(QPainter::Antialiasing);
//    painter.fillRect(event->rect(), QBrush(Qt::white)
    painter.save();
    drawField(painter);
    drawCars(painter);
    painter.restore();

}

void Window::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ShiftModifier){
        shift_pressed = true;
        qInfo() << "shift pressed";
    }

//    else{
        //qInfo() << "pressed buttons " << pressed_buttons;
        QString text = event->text();

        if (text != ""){
            if (text[text.size() - 1] == ' '){
//                qDebug() << "press " + text;
                space_pressed = true;
            }
            else{
//                qDebug() << "press " + text;
                pressed_buttons.append(text[0].toLower());
            }
        }

//    }

}

void Window::keyReleaseEvent(QKeyEvent *event)
{

    if (event->modifiers() == Qt::ShiftModifier ){
//        qInfo() << "release shift";
        shift_pressed = false;
//        qInfo() << "now pressed_buttons is " << pressed_buttons;
    }
//    else{
        QString text = event->text();
        if (text != ""){
            if (text[text.size() - 1] == ' '){
                space_pressed = false;
            }
            else{
//                qInfo() << "at least came here";
//                qInfo() << "event->text() release now is " << text;
                int i = 0;
                while (i < pressed_buttons.size()){
                    if (pressed_buttons[i] == text[0].toLower()) pressed_buttons.erase(pressed_buttons.begin() + i);
                    else ++i;
                }
//                qInfo() << "at least came here 2";
            }
        }

//        qDebug() << "release " + text;
//    }

}

void Window::define_the_direction_ofmoving(){
    ////qInfo() << "try to determine current direction " << pressed_buttons;
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
void Window::change_acceleration_of_speed(){
    if (pressed_buttons.contains('w')) {
        if (speed < max_speed)
        speed += step_speed;
    }
    else if (pressed_buttons.contains('s')){
        if (speed > -max_speed)
        speed += -step_speed;
    }

    else
    {
        if (speed > 0){
            speed -= deceleration;
        }
    }
}

void Window::space_speed_down(){
    if (space_pressed) {
        deceleration = 2 * standard_declaration;
    }
    else {
        deceleration = standard_declaration;
    }
}

void Window::shift_speed_up(){
    if (shift_pressed) {
        step_speed = 2 * standard_step_speed;
        max_speed = 2 * standard_max_speed;
    }
    else {
        step_speed = standard_step_speed;
        max_speed = standard_max_speed;
    }
}

void Window::moveCar(){
//    qDebug() << "try to move car" << "current direction is "<< direction;
    shift_speed_up();
    space_speed_down();
    define_the_direction_ofmoving();
    change_acceleration_of_speed();
    qInfo() << "current speed is " << speed;
    if (collision && blocked_direction.contains(direction)){
        qInfo() << "car blocked";
        qInfo() << "blocked direction are " << blocked_direction;
    ;
    }
    else{
        qInfo() << "try move";
        switch(direction){
            case 0:
                if (speed < 0){
                    x -=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y += qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                    alpha += alpha_step;
                }
                else{

                    x +=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y -= qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                    alpha += alpha_step;
                    alpha_step += alpha_step_step;
                    alpha_step_step = alpha_step_step_scale;
                }

                break;
            case 1:
                if (speed < 0){
                    x -=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y += qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                    alpha += alpha_step;
                }
                else{
                    x +=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y -= qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                    alpha += alpha_step;
                }
                break;
            case 2:
                x +=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                y -= qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                alpha += alpha_step;
    //            if ()
                alpha_step += alpha_step_step;
                alpha_step_step = -alpha_step_step_scale;
                break;
            case 3:
            if (speed > 0){
                x +=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                y -= qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                alpha += alpha_step;
                alpha_step += alpha_step_step;
                alpha_step_step = +alpha_step_step_scale * 0.2;
            }
            else{
                x -=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                y += qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                alpha -= alpha_step;
            }
                break;
            case 4:
                if (speed > 0){
                    x +=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y -= qSin(qDegreesToRadians(alpha)) * qAbs(speed);
//                    alpha += alpha_step;
                }
                else{
                    x -=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y += qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                    alpha += alpha_step;
                }
            break;
            case 5:
                if (speed > 0){
                    x +=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y -= qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                    alpha += alpha_step;
                    alpha_step += alpha_step_step;
                    alpha_step_step = -alpha_step_step_scale * 0.2;
                }
                else{
                    x -=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y += qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                    alpha += alpha_step;
                }
                break;
            default:
                ;
        }
    }


    update();
}
