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
    move_the_car->start(50);
}
void Window::initialize_constant_of_moving(){
    speed = 0.5;
    step_speed = 0.05 * speed;
    deceleration = 0.2 * step_speed;
    alpha = 0;
    alpha_step = 0;
    alpha_step_step = 0;
    alpha_step_step_scale = 0.3;

}
void Window::setCar()
{
//    QPainterPath car;
//    car.clear();
//    car.addRoundedRect(0, 0, 40, 20, 3, 3);
//    this->car = car;
//    update();

//    QImage image ("/home/peter/Race_UI/car.png");


    this->car = *new QPainterPath();
    QRect rect = QRect(x, y,  40,  20);
//    QPoint center = rect   .center();
//    QTransform t = QTransform().translate( x, y ).rotate( -alpha ).translate(-x, -y);
////    QPolygon rotatedRect =  t.mapToPolygon( simulate );
//    QRect rotatedRect = t.mapRect(rect);
//    qInfo() << "drawed rotated rect " << rotatedRect;
//    car.addRoundedRect(rect, 3, 3);
    car.addRect(rect);
//    update();
}

void Window::setField()
{
//    QPainterPath ExternalPath;
//    QPainterPath InternalPath;
    double y_window = size().height();
    double x_window = size().width();
//    qInfo() << "x " << x_window << " y " << y_window;
    center_y_window = y_window/ 2;
    center_x_window = x_window / 2;
////    qInfo() << "try to set field";

//    double marginLeft = x_window / 15;
//    double marginRight = x_window / 15;
//    double marginTop = x_window / 15;
//    double marginBottom = x_window / 15;


//    double width_of_rotation = x_window / 20;
//    ExternalPath.moveTo(marginLeft,0);
//    ExternalPath.lineTo(marginLeft + x_window/4, 0);

//    ExternalPath.arcTo(marginLeft + x_window/4, 0, x_window /8, y_window /8, 90, -90);
//    ExternalPath.arcTo(marginLeft + 3 * x_window /8, y_window/8, x_window/4, y_window/8, 180, 90);
//    ExternalPath.lineTo(x_window - marginRight - width_of_rotation, y_window/4);
//    ExternalPath.arcTo(x_window - marginRight - width_of_rotation, y_window/4, width_of_rotation, width_of_rotation, 90, -90);
//    ExternalPath.lineTo(x_window - marginRight, y_window - marginBottom - width_of_rotation);

//    double width_road = y_window /15;
//    InternalPath.moveTo(marginLeft, width_road);
//    InternalPath.lineTo(marginLeft + x_window/4 - width_road, width_road);
//    InternalPath.arcTo(marginLeft + x_window/4 - width_road, width_road, x_window /8, y_window /8, 90, -90);
//    InternalPath.arcTo(marginLeft + 3 * x_window /8 - width_road, width_road + y_window/8, x_window/4, y_window/8, 180, 90);
//    InternalPath.lineTo(x_window - marginRight - width_road - width_of_rotation, y_window/4 + width_road);
//    InternalPath.arcTo(x_window - marginRight - width_of_rotation - width_road, y_window/4 + width_road, width_of_rotation, width_of_rotation, 90, -90);
//    InternalPath.lineTo(x_window - marginRight - width_road, y_window - marginBottom - width_of_rotation);
////    InternalPath.arcTo(x_window - marginRight - width_road, y_window - marginBottom - width_of_rotation, -width_of_rotation, width_of_rotation, 0, -90);


////    InternalPath.arcTo()

//    ExternalPath.addPath(InternalPath);

//    this->field = ExternalPath;


    QPainterPath roundRectPath;
    roundRectPath.addRoundedRect(0,0, 2 * center_x_window, 2 * center_y_window, 30, 30);
    roundRectPath.addRoundedRect(width_road, width_road, 2 * center_x_window - 2 * width_road, 2 * center_y_window - 2 * width_road,30,30);

    this->field = roundRectPath;

//    update();
}

void Window::drawCar(QPainter &painter)
{
    setCar();
    painter.resetTransform();
//    qInfo() << "try set car position";
//    car.setElementPositionAt(0,x, y);
//    painter.translate(x, y);
//    painter.rotate(-alpha);
//    painter.translate(-x, -y);

    QTransform* t = new QTransform;
    t->translate(x, y).rotate( -alpha ).translate(-x, -y);
    car = t->map(car);

//    QImage image ("/home/peter/Race_UI/car.png");
//    image = image.scaled(20,10);
//    painter.drawImage(10, 10, image);

    if (vertical_collision) painter.fillPath(car, Qt::red);
    else painter.fillPath(car, Qt::blue);

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
    QRect simulate = QRect(x, y,  40, 20);
    QTransform* t = new QTransform;
    t->translate(x, y).rotate( -alpha ).translate(-x, -y);
    simulate = t->mapRect(simulate);

    painter.drawRect(simulate);
//    qInfo() << "topLeft point " << rotatedRect.toRectF().topLeft() << " is contains " << field.contains(rotatedRect.toRectF().topLeft());
//    qInfo() << "topRight point " << rotatedRect.toRectF().topRight() << " is contains " << field.contains(rotatedRect.toRectF().topRight());
//    qInfo() << "topLeft point " << rotatedRect.toRectF().bottomLeft() << " is contains " << field.contains(rotatedRect.toRectF().bottomLeft());
//    qInfo() << "topLeft point " << rotatedRect.toRectF().bottomRight() << " is contains " << field.contains(rotatedRect.toRectF().bottomRight());
//    if (field.contains(rotatedRect.toRectF().topLeft())|| field.contains(rotatedRect.toRectF().bottomLeft()) || field.contains(rotatedRect.toRectF().bottomRight())
//            || field.contains(rotatedRect.toRectF().topRight())){
//        vertical_collision = true;
//    }
//    else {
//        vertical_collision = false;
//    }

//        if (field.intersects(rotatedRect) && !field.contains(rotatedRect)){
//            vertical_collision = true;
//        }
//        else {
//            vertical_collision = false;
//        }
//    field.intersects()
        if (field.intersects(car) && !field.contains(car)){
            vertical_collision = true;
        }
        else {
            vertical_collision = false;
        }

//    painter.setRenderHint(QPainter::Antialiasing);
//    painter.fillRect(event->rect(), QBrush(Qt::white));
//    QLineF line = QLineF(100,400,350,50);

//    if (line.intersects(rotatedRect))
    QPixmap pixmap("/home/peter/Race_UI/car.png");
//    painter.save();
//    QImage image ("/home/peter/Race_UI/car.png");
//    pixmap.fill(Qt::transparent);
    //    painter.drawLine(line);
//    image.set
    setWindowOpacity(0);

//    painter.drawImage(100, 100, image);
//    painter.drawPixmap(QPointF(200, 200), pixmap);
    drawField(painter);
    drawCar(painter);
//    painter.restore();


}


void Window::keyPressEvent(QKeyEvent *event)
{
//    qInfo() << "modifiers is " << event->modifiers();
//    if (event->modifiers() == Qt::ShiftModifier){

//    }
    QString text = event->text();
    qDebug() << "press " + text;
    pressed_buttons.append(text[0]);

}

void Window::keyReleaseEvent(QKeyEvent *event)
{
//    qDebug("At least came here 2");

    QString text = event->text();
    int i = 0;
    while (i < pressed_buttons.size()){
        if (pressed_buttons[i] == text[0]) pressed_buttons.erase(pressed_buttons.begin() + i);
        else ++i;
    }

//    qDebug() << "release " + text;
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
//void Window::change_acceleration_of_speed(){
//    if (pressed_buttons.contains('w')) {
//        if (speed < 3)
//        speed += step_speed;
//    }
//    else {
//        if (speed > 0){
//            speed -= deceleration;
//        }
//    }
//}

void Window::moveCar(){
//    qDebug() << "try to move car" << "current direction is "<< direction;
    define_the_direction_ofmoving();
//    change_acceleration_of_speed();
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
//            if (!vertical_collision) y -= qSin(qDegreesToRadians(alpha)) * speed;
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
