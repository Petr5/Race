#include "window.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QSignalMapper>
#include <QSoundEffect>

Window::Window(Client * client)

{
    this->client = client;
    NickName = client->NickName;
    client->setPointer_to_UI(this);
    width_road = 180;
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

    QTimer* chechk_control_points = new QTimer();

    connect(chechk_control_points, SIGNAL(timeout()), this, SLOT(checkControlPoints()));

    connect(chechk_control_points, SIGNAL(timeout()), this, SLOT(PrintCurrentrating()));
    chechk_control_points->start(50);


    setSoundEffect();
}
static int nmb_packet = -1;
void Window::updateInformationForServer(){

    ++nmb_packet;
    //qInfo() << "sent to server " + QString("300 " + QString::number(x) + " " + QString::number(y) + " " + QString::number(alpha)) + "packet " + QString::number(nmb_packet);
    client->SendToServer(QString("300 " + QString::number(x) + " " + QString::number(y) + " " + QString::number(alpha)) + " packet " + QString::number(nmb_packet));
    client->SendToServer(QString("200 ") + "packet " + QString::number(nmb_packet));
    QString time_intersect_update = "400 ";
    int nmb_visited_points = id_point_to_his_timestamps.size();
    time_intersect_update += QString::number(nmb_visited_points);
    for (int i =0; i < id_point_to_his_timestamps.size(); ++i){
        time_intersect_update.append(" " + QString::number(id_point_to_his_timestamps[i][id_point_to_his_timestamps[i].size() - 1]) );
    }
    client->SendToServer(time_intersect_update + " packet " + QString::number(nmb_packet));
}
void Client::CallUpdateUI(QString message){

    window->CallDrawCars(message);
}
void Client::CallUpdateTimeStamp(QString message){

    window->CallPrintStatistics(message);
}
void Client::start_the_game(){
//   window = new Window(this);
    window->show();
    window->time_of_start_game = QDateTime::currentDateTime();
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
    bool need_to_change_points = false;
    QPainterPath roundRectPath;
    double x_window = size().width();
    if (x_window /2 != center_x_window) {
        need_to_change_points = true;
        qInfo() << "nedd to change points!!!";
    }
    double y_window = size().height();
    center_y_window = y_window / 2;
    center_x_window = x_window / 2;
//    ////qInfo() << "try to set field";
    roundRectPath.addRoundedRect(0,0, 2 * center_x_window, 2 * center_y_window, 30, 30);
    roundRectPath.addRoundedRect(width_road , width_road , 2 * center_x_window - 2 * width_road, 2 * center_y_window - 2 * width_road,30,30);
    double size_of_control_points = width_road / 2;
    if (need_to_change_points) setControlPoints({{width_road / 4, width_road / 4, size_of_control_points, size_of_control_points}, {x_window - width_road ,  width_road / 4, size_of_control_points, size_of_control_points},
                      {x_window - width_road, y_window - width_road, size_of_control_points, size_of_control_points}, {width_road / 4, y_window - width_road , size_of_control_points, size_of_control_points}});
    need_to_change_points = false;
    this->field = roundRectPath;
    update();
}

void Window::setControlPoints(QList<QList<double>> control_points){
//    this->control_points = control_points;
    qInfo()  <<"called setControlPoints";
    nmb_of_contral_points = control_points.size();
    qInfo() << "control points are " << control_points;
    foreach (QList<double> point, control_points) {
        QRect rect = QRect(point[0], point[1], point[2], point[3]);
        QPainterPath path;
        path.addRect(rect);
        this->control_points.append(path);
    }

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
//void Window::
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
void Window::drawControlPoint(QPainter &painter, QPainterPath path){
    painter.resetTransform();
    painter.drawPath(path);
}
void Window::drawControlPoints(QPainter &painter){
    foreach (QPainterPath path, control_points) {
        drawControlPoint(painter, path);
    }
}

void Window::checkControlPoints(){
//    qInfo() << "try check control points";
//    qInfo() << "control_points size " << control_points.size();
    int id = 0;
    foreach (QPainterPath path, control_points) {
        if (path.intersects(car) && !path.contains(car)){
//            time_of_intersection_control_points.append(time_of_start_game.secsTo(QDateTime::currentDateTime()));
//            qInfo() << "id now " << id;
//            if (id < nmb_of_contral_points - 1) qInfo() << "nmb visits are " << id_to_numbs_visit_point[id] << " " << id_to_numbs_visit_point[id + 1];
//            else qInfo() << "nmb visits are " << id_to_numbs_visit_point[id - 1] << " " << id_to_numbs_visit_point[id];
            if (id < nmb_of_contral_points - 1){
                if (id_to_numbs_visit_point[id] > id_to_numbs_visit_point[id + 1]){
                   ;
                }
                else{
                    id_point_to_his_timestamps[id].append(time_of_start_game.secsTo(QDateTime::currentDateTime()));
                    ++id_to_numbs_visit_point[id];
                }
            }
            else if (id == nmb_of_contral_points - 1){
                if (id_to_numbs_visit_point[id] == id_to_numbs_visit_point[id - 1])
                    ;
                else{
                    id_point_to_his_timestamps[id].append(time_of_start_game.secsTo(QDateTime::currentDateTime()));
                    ++id_to_numbs_visit_point[id];
                }
            }
            else{
                qInfo() << "smth went wrong! Need to fix";
            }
        }
        ++id;
    }
}
void Window::PrintStatistics(QPainter &painter){
//    qInfo() << "try to print statistic";
//    qInfo() << "time_of_visiting_control_points is " << time_of_visiting_control_points;
    QString statistic = "Name   ";
    statistic.append("\n");
    for (int i = 0; i < nmb_of_contral_points; ++i){
        statistic.append(" " + QString::number(i));
    }
    statistic.append("\n");


    double size_of_text = 2 * width_road /3;
    QFont font = painter.font() ;

    /* twice the size than the current font size */
    font.setPointSize(3 * font.pointSize() /4);

    /* set the modified font to the painter */
    painter.setFont(font);
    double distance_between_rows_in_table = size_of_text /5;
    painter.drawText(center_x_window - size_of_text, center_y_window- distance_between_rows_in_table, statistic);

    int j = 0;
    for (auto it = time_of_visiting_control_points.begin(); it != time_of_visiting_control_points.end(); ++it){
        QString NickName = it.key();
        QString statistic2;
        statistic2.append(NickName);
        QList<double> timestamps = it.value();
        for (int i = 0; i < timestamps.size(); ++i){
            statistic2.append(" " + QString::number(timestamps[i]));
        }
        painter.drawText(center_x_window - size_of_text, center_y_window + j * distance_between_rows_in_table, statistic2);
        ++j;
    }



}
void Window::CallPrintStatistics(QString message){
//    qInfo() << "called CallPrintStatistics with message" << Qt::endl << message;
    int amount = message.split('\n')[0].split(" ")[1].toInt();
    for (int i = 1; i <= amount; ++i){
        //qInfo() << "i is " << i;
        QStringList row = message.split('\n')[i].split(" ");
//        qInfo() << "curr row is " << row;
        int amount_timestamp = row.size() - 1;
        QList<double> list;
        for (int j = 1; j < amount_timestamp + 1; ++j){
//            qInfo() << "list now " << list;
            list.append(row[j].toDouble());
        }
        time_of_visiting_control_points[row[0]] = list;
//        const QList<double> list = QList<double>({message.split('\n')[i].split(" ")[1].toDouble(), message.split('\n')[i].split(" ")[2].toDouble(), message.split('\n')[i].split(" ")[3].toDouble()});
        //qInfo() << "list is " << list;
    }
}
void Window::fill_road(QPainter &painter){
//    roundRectPath.addRoundedRect(0,0, 2 * center_x_window, 2 * center_y_window, 30, 30);
//    roundRectPath.addRoundedRect(width_road , width_road , 2 * center_x_window - 2 * width_road, 2 * center_y_window - 2 * width_road,30,30);
    QImage image("/home/peter/Race/road.jpeg");
//    double
    image.scaled(width_road, width_road);
    for (int i = 0; i < center_x_window / width_road; ++i){
        painter.drawImage(i * width_road, 0, image);

    }
    double step = width_road / 6;
    int nmb_step = center_x_window / step;
    double alpha = 0;
    double alpha_step = 90 / nmb_step;
    for (int i = 0; i < nmb_step; ++i){
        painter.resetTransform();
        painter.translate(center_x_window + i * step, 0);

        painter.rotate(alpha);
        painter.drawImage(0, 0, image);
        alpha += alpha_step;

    }
//    painter.drawImage(0, 0, image);
//    painter.drawImage(center_x_window, 0, image);


}
void Window::PrintCurrentrating(){

//    qInfo() << "current time points are " << id_point_to_his_timestamps;
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
    setField();

    if (!collision){
//        qInfo() << "after last collision " << time_of_colision.msecsTo(QDateTime::currentDateTime()) ;
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
//    fill_road(painter);
    drawCars(painter);
    drawControlPoints(painter);
    PrintStatistics(painter);
    painter.restore();

}

void Window::keyPressEvent(QKeyEvent *event)
{
//    shift_pressed = false;
    if (event->modifiers() == Qt::ShiftModifier && !event->isAutoRepeat()){
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
            if (speed < 0) direction = 3;
            else direction = 0;
        }
        else if (signal == "d"){
            if (speed < 0) direction =5;
            else direction = 2;
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

bool need_to_play_sound = true;

void Window::setSoundEffect(){
    effect = new QSoundEffect();
    effect->setSource(QUrl::fromLocalFile("/home/peter/Race/mixkit-fast-car-drive-by-1538.wav"));
    effect->setLoopCount(QSoundEffect::Infinite);
    effect->setVolume(0.25f);
}
void Window::shift_speed_up(){
    if (shift_pressed) {
        qInfo() << "called speed up " << "shift pressed " << shift_pressed;
        step_speed = 2 * standard_step_speed;
        max_speed = 2 * standard_max_speed;

//        if (need_to_play_sound) {
////            effect->setLoopCount(QSoundEffect::Infinite);
//            effect->play();
//            need_to_play_sound = false;
//        }
    }
    else {
        step_speed = standard_step_speed;
        max_speed = standard_max_speed;
//        if (!need_to_play_sound){
////            effect->stop();
////            effect->setLoopCount(0);
//            need_to_play_sound = true;
//        }
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
//        qInfo() << "car blocked";
//        qInfo() << "blocked direction are " << blocked_direction;
    ;
    }
    else{
//        qInfo() << "try move";
        switch(direction){
            case 0:
                if (speed < 0){
                    x -=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y += qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                    alpha += alpha_step;
                    alpha_step += alpha_step_step;
                    alpha_step_step = alpha_step_step_scale;
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
                if (speed < 0){
                    x -=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y += qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                    alpha += alpha_step;
                    alpha_step += alpha_step_step;
                    alpha_step_step = -alpha_step_step_scale;
                }

                else{
                    x +=  qCos(qDegreesToRadians(alpha)) * qAbs(speed);
                    y -= qSin(qDegreesToRadians(alpha)) * qAbs(speed);
                    alpha += alpha_step;
        //            if ()
                    alpha_step += alpha_step_step;
                    alpha_step_step = -alpha_step_step_scale;
                }
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
                alpha += alpha_step;
                alpha_step += alpha_step_step;
                alpha_step_step = alpha_step_step_scale;
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
                    alpha_step += alpha_step_step;
                    alpha_step_step = -alpha_step_step_scale;
                }
                break;
            default:
                ;
        }
    }


    update();
}
