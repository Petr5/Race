// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "client.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QMessageBox>

Client::Client(QString serverName, QDialog* dialog, QString NickName, QWidget* parent)
    : QWidget(parent), nextBlockSize(0), dialog(dialog), NickName(NickName)
    // Устанавливаем nextBlockSize равным нулю
{
    // Инициализируем сокет
    localSocket = new QLocalSocket(this);

    // Устанавливаем соединение между сигналом ошибки сокета с обработчиком ошибок
    connect(localSocket, &QLocalSocket::errorOccurred, this, &Client::slotError);

    localSocket->bytesToWrite();
    // Устанавливаем имя сервера, к которому сокет должен подключаться
    localSocket->setServerName(serverName);

    // Устанавливаем соединение между сигналом подключения сокета к серверу
    // и обработчиком сигнала
    connect(localSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(localSocket, SIGNAL(connected()), dialog, SLOT(slotClientConnected()));

    // Соединяем сигнал сокета о готовности приёма данных данных с обработчиком данных
    connect(localSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

    // Инициализируем элементы интерфейса
    textEdit = new QTextEdit;
    sendRevision = new QPushButton("Send next revision");

    // Соединяем нажатие кнопки с обработчиком, передающим информацию о ревизии на сервер
//    connect(sendRevision, SIGNAL(clicked()), this, SLOT(slotSendToServer()));

    // Настраиваем элементы интерфейса и формируем вид окна клиента
    textEdit->setReadOnly(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Sender revisions"));
    layout->addWidget(textEdit);
    layout->addWidget(sendRevision);
    setLayout(layout);

    // Подключаем сокет к серверу
    localSocket->connectToServer();
}

Client::~Client()
{

}

// Слот чтения информации, получаемой от сервера
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

// Слот передачи информации на сервер
void Client::slotSendToServer(QString command)
{
    // Блок для передачи формируется аналогично тому, как это делается на сервере
    QByteArray arrayBlock;
    QDataStream out(&arrayBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    qInfo() << command;

    QString message = command;
    out << quint16(0) << QTime::currentTime() << NickName + ": " + message;

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

    qInfo() << "try send to server " + command;

    QString message = command;
    out << quint16(0) << QTime::currentTime() << message;

    out.device()->seek(0);
    out << quint16(arrayBlock.size() - sizeof(quint16));

    localSocket->write(arrayBlock);
}

// Слот обработки сигнала соединения с сервером
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
    qInfo() << "try to present yourself";
//    NickName = "Peter";
    QString command = "100 " + NickName;
//    while (!server_authenticate){
        SendToServer(command);
//    }
}
