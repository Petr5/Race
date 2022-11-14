// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "server.h"

#include <QtWidgets>
#include <QtNetwork>

Server::Server(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QString serverName("RaceServer");

    QLocalServer::removeServer(serverName);

    server = new QLocalServer(this);
    if (!server->listen(serverName)) {
        QMessageBox::critical(this, tr("Local Race Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        close();
        return;
    }

    statusLabel = new QLabel;
    statusLabel->setWordWrap(true);
    statusLabel->setText(tr("The server is running.\n"));

    fortunes << tr("ACK")
             << tr("ACK2");

    QPushButton *quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    connect(quitButton, &QPushButton::clicked, this, &Server::close);
//    connect(server, &QLocalServer::newConnection, this, &Server::sendFortune);

//        connect(server, &QLocalServer::newConnection, this, &Server::SendACK);
//    connect(server, &QLocalServer::newConnection, this, &Server::ReadClient);
    connect(server, &QLocalServer::newConnection, this, &Server::slotNewConnection);


    textEdit = new QTextEdit;
    textEdit->setReadOnly(true);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(textEdit);

    setWindowTitle(QGuiApplication::applicationDisplayName());
//    QLocalSocket *clientConnection = server->nextPendingConnection();
//    server->
//    qInfo() << clientConnection;
//    connect(clientConnection, SIGNAL(ReadyRead()), this, SLOT(ReadClient()));
//    connect(server, &QLocalServer::readyRead, this, &Client::readFortune);
}

//void Server::SendACK(){
//    qInfo() << "try to sentd ack";
//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    const QString &message = fortunes.at(0);
//    out << quint32(message.size());
//    out << message;

//    QLocalSocket *clientConnection = server->nextPendingConnection();
//    connect(clientConnection, &QLocalSocket::disconnected,
//            clientConnection, &QLocalSocket::deleteLater);

//    clientConnection->write(block);
//    clientConnection->flush();
//    clientConnection->disconnectFromServer();
//}

void Server::ReadClient(){
    qInfo() << "try to read data from client";
    QLocalSocket *sock = server->nextPendingConnection();

    if( !sock->waitForConnected() )
        return;

    if( !sock->waitForReadyRead() )
        return;

    QByteArray incoming(sock->readAll());
    qInfo() << "read from client" << incoming;
//    QMessageBox::information(this, "read from client", incoming);
}\


void Server::slotNewConnection()
{
    // Получаем сокет, подключённый к серверу
    QLocalSocket* localSocket = server->nextPendingConnection();
    // Соединяем сигнал отключения сокета с обработчиком удаления сокета
    connect(localSocket, SIGNAL(disconnected()), localSocket, SLOT(deleteLater()));
    // Соединяем сигнал сокета о готовности передачи данных с обработчиком данных
    connect(localSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    // Отправляем информацию клиенту о соединении с сервером
    sendToClient(localSocket, "Server response: Connected!");
}

// Слот чтения информации от клиента
void Server::slotReadClient()
{
    // Получаем QLocalSocket после срабатывания сигнала о готовности передачи данных
    QLocalSocket* localSocket = (QLocalSocket*)sender();
    // Создаём входной поток получения данных на основе сокета
    QDataStream in(localSocket);
    // Устанавливаем версию сериализации данных потока. У клиента и сервера они должны быть одинаковыми
    in.setVersion(QDataStream::Qt_5_3);
    // Бесконечный цикл нужен для приёма блоков данных разных размеров, от двух байт и выше
    for(;;)
    {
        // Если размер блока равен нулю
        if(!nextBlockSize)
        {
            // Если размер передаваемого блока меньше двух байт, выйти из цикла
            if(localSocket->bytesAvailable() < (int)sizeof(quint16))
                break;
            // Извлекаем из потока размер блока данных
            in >> nextBlockSize;
        }

        // Извлекаем из потока время и строку
        QTime time;
        QString string;
        in >> time >> string;

        // Преобразуем полученные данные и показываем их в виджете
        QString message = time.toString() + " " + "Client has sent - " + string;
        QString code = string.split(" ")[0];
        qInfo() << "code of this message is " << code;
        if (code == "100"){
            qInfo() << "try Nickname for current user";
            NickName = string.split(" ")[1];
            qInfo() << "Nickname for current user is " << NickName;
            SendACKName(localSocket);
        }
        else if (code == "150"){
            server->disconnect();
        }
        textEdit->append(message);

        nextBlockSize = 0;

        // Отправляем ответ клиенту

        sendToClient(localSocket, "Server response: received \"" + string + "\"");
//        SendACK(localSocket);

//        SendACK(localSocket);
    }
}

// Метод для отправки клиенту подтверждения о приёме информации
void Server::sendToClient(QLocalSocket* localSocket, const QString& string)
{
    // Поскольку заранее размер блока неизвестен (параметр string может быть любой длины),
    // вначале создаём объект array класса QByteArray
    QByteArray array;
    // На его основе создаём выходной поток
    QDataStream out(&array, QIODevice::WriteOnly);
    // Устанавливаем версию сериализации данных потока
    out.setVersion(QDataStream::Qt_5_3);
    // Записываем в поток данные для отправки. На первом месте идёт нулевой размер блока
    out << quint16(0) << QTime::currentTime() << string;

    // Перемещаем указатель на начало блока
    out.device()->seek(0);
    // Записываем двухбайтное значение действительного размера блока без учёта пересылаемого размера блока
    out << quint16(array.size() - sizeof(quint16));

    // Отправляем получившийся блок клиенту
    localSocket->write(array);
}

void Server::SendACK(QLocalSocket* localSocket){
    sendToClient(localSocket, "ACK");
}

void Server::SendACKName(QLocalSocket* localSocket){
    sendToClient(localSocket, "ACK Name");
}
void Server::SendACKPosition(QLocalSocket* localSocket){
    sendToClient(localSocket, "ACK Position");
}
