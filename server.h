// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QLocalSocket>
#include <QTextEdit>
QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLocalServer;
QT_END_NAMESPACE

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    quint16 nextBlockSize;
private slots:
    void ReadClient();
    void slotReadClient();

    void slotNewConnection();
private:
    QString NickName;
    QLocalServer *server;
    QStringList fortunes;
    QLabel *statusLabel;
//    void SendACK();
    QTextEdit* textEdit;
    void sendToClient(QLocalSocket *localSocket, const QString &string);
    void SendACK(QLocalSocket *localSocket);
    void SendACKName(QLocalSocket *localSocket);
    void SendACKPosition(QLocalSocket *localSocket);
};

#endif
