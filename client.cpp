// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "client.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QMessageBox>
#include <QSignalMapper>

Client::Client(QString serverName, QDialog* dialog, QString NickName, QWidget* parent)
    : QWidget(parent), nextBlockSize(0), dialog(dialog), NickName(NickName)
    // Устанавливаем nextBlockSize равным нулю
{
    // Инициализируем сокет
    localSocket = new QLocalSocket(this);

//    QString serverName = "RaceServer";
    // Устанавливаем соединение между сигналом ошибки сокета с обработчиком ошибок
    connect(localSocket, &QLocalSocket::errorOccurred, this, &Client::slotError);

    localSocket->bytesToWrite();
    qInfo() << "server name " << serverName;
    // Устанавливаем имя сервера, к которому сокет должен подключаться
    localSocket->setServerName(serverName);

    // Устанавливаем соединение между сигналом подключения сокета к серверу
    // и обработчиком сигнала
    connect(localSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(localSocket, SIGNAL(connected()), dialog, SLOT(slotClientConnected()));

    connect(localSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

    // Инициализируем элементы интерфейса
    textEdit = new QTextEdit;

    // Настраиваем элементы интерфейса и формируем вид окна клиента
    textEdit->setReadOnly(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(textEdit);
    setLayout(layout);

    // Подключаем сокет к серверу
    localSocket->connectToServer();
}

Client::~Client()
{

}


void Client::setPointer_to_UI(Window* window){
    this->window = window;
}
void Client::slotReadyRead()
{
    // Всё аналогично приёму информации на стороне сервера
    QDataStream in(localSocket);
    in.setVersion(QDataStream::Qt_5_3);
//    present_yourself();
    for(;;)
    {
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
        textEdit->append(time.toString() + " " + string);
        nextBlockSize = 0;
    }
}

// Слот обработки ошибок сокета
void Client::slotError(QLocalSocket::LocalSocketError error)
{
    QString strError =
        "Error: " + (error == QLocalSocket::ServerNotFoundError ?
                     "The server was not found." :
                     error == QLocalSocket::PeerClosedError ?
                     "The server is closed." :
                     error == QLocalSocket::ConnectionRefusedError ?
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
