// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "server.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QMessageBox>

Server::Server(QWidget *parent, int amount)
    : QTcpServer(parent), amount(amount)
{
    QDialog* dialog = new QDialog;
    QGridLayout* layout = new QGridLayout();
    dialog->setLayout(layout);
    QPushButton* button = new QPushButton;
    QLabel* label = new QLabel;
    QLineEdit* lineEdit = new QLineEdit;

    label->setText(tr("Enter amount of players"));
    layout->addWidget(label,0,0);
    layout->addWidget(button,0,1);
    layout->addWidget(lineEdit, 1,0);
    connect(button, &QPushButton::clicked, [=](){
        this->amount = (lineEdit->text()).toInt();
        dialog->hide();
    });
    dialog->show();

    QHostAddress* addr = new QHostAddress("127.0.0.1");

    setSslLocalCertificate("sslserver.pem");
    setSslPrivateKey("sslserver.key");
    setSslProtocol(QSsl::TlsV1_2);
    if (!listen(*addr, 3333)) {
//        QMessageBox::critical(this, tr("Local Race Server"),
//                              tr("Unable to start the server: %1.")
//                              .arg(server->errorString()));
        close();
        return;
    }

}

void Server::incomingConnection(qintptr socketDescriptor){

    localSocket = new QSslSocket((QTcpServer *)this);
//    localSocket->addCaCertificate("")
    localSocket->setSocketDescriptor(socketDescriptor);
    localSocket->setLocalCertificate(m_sslLocalCertificate);
    localSocket->setPrivateKey(m_sslPrivateKey);
    localSocket->setProtocol(m_sslProtocol);
    localSocket->startServerEncryption();

    ((QTcpServer *)this)->connect((QTcpServer *)this, SIGNAL(newConnection()), (QTcpServer *)this, SLOT(slotNewConnection()));

    this->addPendingConnection(localSocket);
}
void Server::setSslLocalCertificate(const QSslCertificate &certificate)
{
    m_sslLocalCertificate = certificate;
}

bool Server::setSslLocalCertificate(const QString &path, QSsl::EncodingFormat format)
{
    QFile certificateFile(path);

    if (!certificateFile.open(QIODevice::ReadOnly))
        return false;

    m_sslLocalCertificate = QSslCertificate(certificateFile.readAll(), format);
    return true;
}


void Server::setSslPrivateKey(const QSslKey &key)
{
    m_sslPrivateKey = key;
}

bool Server::setSslPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm, QSsl::EncodingFormat format, const QByteArray &passPhrase)
{
    QFile keyFile(fileName);

    if (!keyFile.open(QIODevice::ReadOnly))
        return false;

    m_sslPrivateKey = QSslKey(keyFile.readAll(), algorithm, format, QSsl::PrivateKey, passPhrase);
    return true;
}


void Server::setSslProtocol(QSsl::SslProtocol protocol)
{
    m_sslProtocol = protocol;
}


//void Server::initializeServerConstant(){
//    Server::cuurent_id_user = 0;
//}
int Server::cuurent_id_user = 0;

void Server::ReadClient(){
    qInfo() << "try to read data from client";
    QSslSocket *sock = dynamic_cast<QSslSocket*>(nextPendingConnection());

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
    QTcpSocket *socket = nextPendingConnection();
    qInfo() << socket;

    QSslSocket* localSocket = dynamic_cast<QSslSocket*>(socket);
    qInfo() << localSocket;
    ((QTcpServer *)this)->connect(localSocket, SIGNAL(readyRead()), (QTcpServer *)this, SLOT(slotReadClient()));
//    ((QTcpServer *)this)->connect(localSocket, SIGNAL(encrypted()), (QTcpServer *)this, SLOT(slotReadClient()));

    sockets_of_clients.append(localSocket);
    if (Check_full_lobby()){
        qInfo() << "lobby is full";
        foreach (QSslSocket* sock, sockets_of_clients) {

            sendToClient(sock, "250 All connected");
        }
    }
    ((QTcpServer *)this)->connect(localSocket, SIGNAL(disconnected()), localSocket, SLOT(deleteLater()));
//    sendToClient(localSocket, "Server response: Connected!");
}

bool Server::Check_full_lobby(){
    if (sockets_of_clients.size() >= amount){
        return true;
    }
    else return false;
}

bool is_sent_endgame = false;
void Server::slotReadClient()
{

//    ((QTcpServer *)this)->sender();
    QSslSocket* localSocket = (QSslSocket*)sender();

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
//                qInfo() << "try Nickname for current user";
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
                if (!is_sent_endgame){
                    foreach(QSslSocket* socket, sockets_of_clients){
                        QString message = "600 " + NickName;
                        qInfo() << "serverr sent to client " << message;
                        sendToClient(socket, message);
                    }
                    is_sent_endgame = true;
                }

            }
            else{
//                QMessageBox::critical(this, "unknown code", code);
                ;
            }

            QString message = time.toString() + " " + "Client has sent - " + string;
//            textEdit->append(message);

            nextBlockSize = 0;

        }
        else{
            QString message = time.toString() + " " + "Client has sent - " + string;

            nextBlockSize = 0;

//            textEdit->append(message);
            sendToClient(localSocket, "Server response: received \"" + string + "\"");

        }

    }
}

// Метод для отправки клиенту подтверждения о приёме информации
void Server::sendToClient(QSslSocket* localSocket, const QString& string)
{
    qInfo() << "try sent to client " << string;
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
     out << QTime::currentTime() << string;
    // Отправляем получившийся блок клиенту
    qInfo() << "try to localSocket->write";
    localSocket->write(array);
}

void Server::SendACK(QSslSocket* localSocket){
    sendToClient(localSocket, "ACK");
}

void Server::SendACKName(QSslSocket* localSocket){
    sendToClient(localSocket, "ACK Name");
}
void Server::SendACKPosition(QSslSocket* localSocket){
    sendToClient(localSocket, "ACK Position");
}

void Server::sendPlayersPosition(QSslSocket* localSocket, int nmb_packet){
    QString message = "211 " + QString::number(coordinates_of_players.size()) + '\n';
    for (auto it = coordinates_of_players.begin(); it != coordinates_of_players.end(); ++it){
        message += it.key() + " " + QString::number(it.value()[0]) + " " + QString::number(it.value()[1]) + " " + QString::number(it.value()[2]) + '\n';
    }
    message += "nmb_packet is " + QString::number(nmb_packet);
    sendToClient(localSocket, message);
}

void Server::sendPlayersTimeVisitedCPoints(QSslSocket* localSocket, int nmb_packet){
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

