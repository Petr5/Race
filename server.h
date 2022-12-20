// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QSslSocket>
#include <QTextEdit>
#include <QTimer>
#include <QTcpServer>
#include <QSsl>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslSocket>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTcpServer;
QT_END_NAMESPACE



class Server : public QTcpServer
{
    Q_OBJECT

protected:
    void incomingConnection(qintptr socketDescriptor) override final;
public:
    explicit Server(QWidget *parent = nullptr, int amount = 1);
    int amount = 1;
    quint16 nextBlockSize;
private slots:
    void ReadClient();
    void slotReadClient();

    void slotNewConnection();
private:
    QSslSocket* localSocket;
    QSslCertificate m_sslLocalCertificate;
    QSslKey m_sslPrivateKey;
    QSsl::SslProtocol m_sslProtocol;

    static int cuurent_id_user;
    QList<QString> NickNames;
//    QString NickName;
    QList<QSslSocket*> sockets_of_clients;
    QMap<QString, QList<double>> coordinates_of_players;
    QMap<QString, QList<double>> time_of_visiting_control_points;
    QTcpServer *server;
    QStringList fortunes;
    QLabel *statusLabel;
//    void SendACK();
    QTextEdit* textEdit;
    void sendToClient(QSslSocket *localSocket, const QString &string);
    void SendACK(QSslSocket *localSocket);
    void SendACKName(QSslSocket *localSocket);
    void SendACKPosition(QSslSocket *localSocket);
//    void initializeServerConstant();
    void sendPlayersPosition(QSslSocket *localSocket, int nmb_packet);

    bool Check_full_lobby();
    void sendPlayersTimeVisitedCPoints(QSslSocket *localSocket, int nmb_packet);

    void setSslLocalCertificate(const QSslCertificate &certificate);
    bool setSslLocalCertificate(const QString &path, QSsl::EncodingFormat format = QSsl::Pem);

    void setSslPrivateKey(const QSslKey &key);
    bool setSslPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm = QSsl::Rsa, QSsl::EncodingFormat format = QSsl::Pem, const QByteArray &passPhrase = QByteArray());

    void setSslProtocol(QSsl::SslProtocol protocol);
};


#endif
