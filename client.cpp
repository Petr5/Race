// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "client.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QMessageBox>
#include <QSignalMapper>

Client::Client(QString serverName, int port, ConnectDialog* dialog, QString NickName, QWidget* parent)
    : QWidget(parent), nextBlockSize(0), dialog(dialog), NickName(NickName)
    // Устанавливаем nextBlockSize равным нулю
{
    // Инициализируем сокет
    localSocket = new QTcpSocket(this);

//    QString serverName = "RaceServer";
    // Устанавливаем соединение между сигналом ошибки сокета с обработчиком ошибок
    connect(localSocket, &QTcpSocket::errorOccurred, this, &Client::slotError);

    localSocket->bytesToWrite();
    qInfo() << "server name " << serverName;

    // Устанавливаем соединение между сигналом подключения сокета к серверу
    // и обработчиком сигнала
    connect(localSocket, SIGNAL(connected()), SLOT(slotConnected()));
//    connect(localSocket, SIGNAL(connected()), (QObject*)dialog, SLOT(slotClientConnected()));
    connect(localSocket, SIGNAL(connected()), (QObject*)dialog, SLOT(close()));


    connect(localSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

    // Инициализируем элементы интерфейса
    textEdit = new QTextEdit;

    // Настраиваем элементы интерфейса и формируем вид окна клиента
    textEdit->setReadOnly(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(textEdit);
    setLayout(layout);

    // Подключаем сокет к серверу
//    localSocket->connectToServer();
    localSocket->connectToHost(serverName, port);
}

Client::~Client()
{

}

//void Client::setInteractor(class interactor* interactor){
//    this->interactor = interactor;
//}
void Client::setPointer_to_UI(Window* window){
    this->window = window;
}



void Client::slotReadyRead()
{
//    qInfo() << "called slot readyRead";
    // Всё аналогично приёму информации на стороне сервера
    QDataStream in(localSocket);
    in.setVersion(QDataStream::Qt_5_3);
//    present_yourself();
    for(;;)
    {
//        qInfo() << "nextBlockSize now is " << nextBlockSize;
        if(!nextBlockSize)
        {
            if(localSocket->bytesAvailable() < (int)sizeof(quint16))
                break;
        }
        in >> nextBlockSize;

        if(localSocket->bytesAvailable() < nextBlockSize)
            break;

        QTime time;
        QString string;
        in >> time >> string;
        QString code = string.split(" ")[0];
        if (code == "ACK"){
            if (string.split(" ")[1] == "Name") {
                server_authenticate = true;
                timer->stop();
            }
        }
        else if (code == "211"){
            //qInfo() << "information about other players \n" << string;
            CallUpdateUI(string);
        }
        else if (code == "411"){
//            qInfo() << "code 411 ack";
            CallUpdateTimeStamp(string);
        }
        else if (code == "250"){
//            qInfo() << "code 250 ack!!!!!!!!!!!";
//            interactor;
            start_the_game();

//            startTimer()
        }
        else if (code == "600"){
//            window
            qInfo() << "message accepted from server is " << string;
            QString winner = string.split(" ")[1];
            CalldrawFinalTable(winner);
        }

        textEdit->append(time.toString() + " " + string);
        nextBlockSize = 0;
    }
}


//void Client::start_the_game(){
//    Window *window = new Window(this);
//    window->show();
//}
// Слот обработки ошибок сокета
void Client::slotError(QTcpSocket::SocketError error)
{
    QString strError =
        "Error: " + (error == QTcpSocket::HostNotFoundError ?
                     "The server was not found." :
                     error == QTcpSocket::RemoteHostClosedError ?
                     "The server is closed." :
                     error == QTcpSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(localSocket->errorString()));
    textEdit->append(strError);
}

void Client::slotSendToServer(QString command)
{
    // Блок для передачи формируется аналогично тому, как это делается на сервере
    QByteArray arrayBlock;
    QDataStream out(&arrayBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    //qInfo() << command;
//    qInfo() << "client sent to server " << command;
    QString message = command;
    out << quint16(0) << QTime::currentTime() << NickName + " " + message;

    out.device()->seek(0);
    out << quint16(arrayBlock.size() - sizeof(quint16));

    localSocket->write(arrayBlock);
}

void Client::SendToServer(QString command)
{
    // Блок для передачи формируется аналогично тому, как это делается на сервере
    QByteArray arrayBlock;
    QDataStream out(&arrayBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    //qInfo() << "try send to server " + command;

    QString message = command;
    out << quint16(0) << QTime::currentTime() << NickName + " " + message;

    out.device()->seek(0);
    out << quint16(arrayBlock.size() - sizeof(quint16));

    localSocket->write(arrayBlock);
}

void Client::slotConnected()
{


    textEdit->append("Received the connected() signal");
//    present_yourself();
    timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(updateInformationForServer()));
   connect(timer, SIGNAL(timeout()), this, SLOT(present_yourself()));

//    slotSendToServer();
    timer->start(5000);
}

void Client::present_yourself(){
    //qInfo() << "try to present yourself";
//    NickName = "Peter";
    QString command = "100";
//    while (!server_authenticate){
        SendToServer(command);
//    }
}
