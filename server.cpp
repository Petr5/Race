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

    QHostAddress* addr = new QHostAddress("127.0.0.1");

    server = new QTcpServer(this);
    if (!server->listen(*addr, 3333)) {
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

    connect(server, &QTcpServer::newConnection, this, &Server::slotNewConnection);


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

//    initializeServerConstant();

//    QTimer* sendPlayerPosition_timer = new QTimer();
//    connect(sendPlayerPosition_timer, SIGNAL(timeout()), this, SLOT(updateInformationForServer()));
//    sendPlayerPosition_timer->start(5000);
}


//void Server::initializeServerConstant(){
//    Server::cuurent_id_user = 0;
//}
int Server::cuurent_id_user = 0;

void Server::ReadClient(){
    //qInfo() << "try to read data from client";
    QTcpSocket *sock = server->nextPendingConnection();

    if( !sock->waitForConnected() )
        return;

    if( !sock->waitForReadyRead() )
        return;

    QByteArray incoming(sock->readAll());
    //qInfo() << "read from client" << incoming;
//    QMessageBox::information(this, "read from client", incoming);
}\


void Server::slotNewConnection()
{
    qInfo() << "slot new connection triggered";
    QTcpSocket* localSocket = server->nextPendingConnection();

    sockets_of_clients.append(localSocket);
    if (Check_full_lobby()){
        foreach (QTcpSocket* sock, sockets_of_clients) {

            sendToClient(sock, "250 All connected");
        }
    }
    connect(localSocket, SIGNAL(disconnected()), localSocket, SLOT(deleteLater()));
    connect(localSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    sendToClient(localSocket, "Server response: Connected!");
}

bool Server::Check_full_lobby(){
    if (sockets_of_clients.size() >= 2){
        return true;
    }
    else return false;
}

void Server::slotReadClient()
{
    QTcpSocket* localSocket = (QTcpSocket*)sender();

    QDataStream in(localSocket);
    in.setVersion(QDataStream::Qt_5_3);
    for(;;)
    {
        if(!nextBlockSize)
        {
            // Если размер передаваемого блока меньше двух байт, выйти из цикла
            if(localSocket->bytesAvailable() < (int)sizeof(quint16))
                break;
            // Извлекаем из потока размер блока данных
            in >> nextBlockSize;
        }

        QTime time;
        QString string;
        in >> time >> string;

        if (string != ""){
            qInfo() << "aaept stering from client " << string;
            QString NickName = string.split(" ")[0];
            QString code = string.split(" ")[1];

            if (code == "100"){
                //qInfo() << "try Nickname for current user";
    //            NickNames = string.split(" ")[1];
    //            NickNames[Server::cuurent_id_user++] =  string.split(" ")[1];
    //            NickNames.append(string.split(" ")[1]);

    //            //qInfo() << "Nickname for current user is " << NickNames[Server::cuurent_id_user++];
                SendACKName(localSocket);
            }
            else if (code == "150"){
                server->disconnect();
            }
            else if (code == "200"){
                ;
                sendPlayersPosition(localSocket, string.split(" ")[3].toInt());
                sendPlayersTimeVisitedCPoints(localSocket, string.split(" ")[3].toInt());
            }
            else if (code == "300"){
//                coordinates_of_players[NickName] = qMakePair(string.split(" ")[2].toInt(), string.split(" ")[3].toInt());
                //qInfo() << "coordinate x from client " << string.split(" ")[2] << "coordinate y from client " << string.split(" ")[3];
                const QList<double> list = QList<double>({string.split(" ")[2].toDouble(), string.split(" ")[3].toDouble(), string.split(" ")[4].toDouble()});
                qInfo() << "list is " << list;
                qInfo() << "server coordinates update now " << list[0] << " " << list[1] << " " << list[2];
                coordinates_of_players.insert(NickName, list);
                qInfo() << "now coordinates_of_players are " << coordinates_of_players;
                ;
            }
            else if(code == "400"){
                qInfo() << "400 accept " << string;
                int cnt = string.split(" ")[2].toInt();
                QList<double> list;
                for (int i = 3; i < 3 + cnt; ++i){
                    list.append(string.split(" ")[i].toDouble());
                }
//                const QList<double> list = QList<double>({string.split(" ")[2].toDouble(), string.split(" ")[3].toDouble(), string.split(" ")[4].toDouble()});
                qInfo() << list;
                time_of_visiting_control_points[NickName] = list;
                qInfo() << "now time_of_visiting_control_points for " + NickName + "is " << time_of_visiting_control_points[NickName];
            }
            else if (code == "600"){
                foreach(QTcpSocket* socket, sockets_of_clients){
                    QString message = "600 " + NickName;
                    qInfo() << "serverr sent to client " << message;
                    sendToClient(socket, message);
                }

            }
            else{
//                QMessageBox::critical(this, "unknown code", code);
                ;
            }

            QString message = time.toString() + " " + "Client has sent - " + string;
            textEdit->append(message);

            nextBlockSize = 0;

            // Отправляем ответ клиенту

//            sendToClient(localSocket, "Server response: received \"" + string + "\"");
    //        SendACK(localSocket);

    //        SendACK(localSocket);
        }
        else{
            //qInfo() << "send empty string";
            QString message = time.toString() + " " + "Client has sent - " + string;

            nextBlockSize = 0;

            textEdit->append(message);
            sendToClient(localSocket, "Server response: received \"" + string + "\"");

        }

    }
}

// Метод для отправки клиенту подтверждения о приёме информации
void Server::sendToClient(QTcpSocket* localSocket, const QString& string)
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
//    out.

//    localSocket->write();
    // Перемещаем указатель на начало блока
    out.device()->seek(0);
    // Записываем двухбайтное значение действительного размера блока без учёта пересылаемого размера блока
    out << quint16(array.size() - sizeof(quint16));

    // Отправляем получившийся блок клиенту
    localSocket->write(array);
}

void Server::SendACK(QTcpSocket* localSocket){
    sendToClient(localSocket, "ACK");
}

void Server::SendACKName(QTcpSocket* localSocket){
    sendToClient(localSocket, "ACK Name");
}
void Server::SendACKPosition(QTcpSocket* localSocket){
    sendToClient(localSocket, "ACK Position");
}

void Server::sendPlayersPosition(QTcpSocket* localSocket, int nmb_packet){
    QString message = "211 " + QString::number(coordinates_of_players.size()) + '\n';
    for (auto it = coordinates_of_players.begin(); it != coordinates_of_players.end(); ++it){
        message += it.key() + " " + QString::number(it.value()[0]) + " " + QString::number(it.value()[1]) + " " + QString::number(it.value()[2]) + '\n';
    }
    message += "nmb_packet is " + QString::number(nmb_packet);
    sendToClient(localSocket, message);
}

void Server::sendPlayersTimeVisitedCPoints(QTcpSocket* localSocket, int nmb_packet){
    QString message = "411 " + QString::number(time_of_visiting_control_points.size()) + '\n';
    for (auto it = time_of_visiting_control_points.begin(); it != time_of_visiting_control_points.end(); ++it){
        QList<double> visited_points = it.value();
        int cnt_visited_control_points = visited_points.size();
        message += it.key();
        for (int i = 0; i < cnt_visited_control_points; ++i){
            message +=  " " + QString::number(visited_points[i]);
        }
        message += '\n';
    }
    message += "nmb_packet is " + QString::number(nmb_packet);
    sendToClient(localSocket, message);
}

