// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CLIENT_H
#define CLIENT_H
//#include "window.h"
#include <QWidget>
#include <QLocalSocket>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>
//#ifndef WINDOW_H
//#include "window.h"
//#define WINDOW_H
//#endif
class Window;
class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QString serverName, QWidget* parent = 0);
    ~Client();
    Window* window;


private:
    // Указатель на QLocalSocket
    QLocalSocket* localSocket;

    // Указатели на элементы интерфейса
    QTextEdit* textEdit;
    QPushButton* sendRevision;

    // Размер принимаемого от сервера блока
    quint16 nextBlockSize;

    // Номер ревизии, отправляемый серверу
    // Увеличивается при каждом нажатии QPushButton
    int revision;
    void SendToServer(QString command);
//    void present_yourself();
    bool server_authenticate = false;
    QTimer* timer;
public slots:
//    void slotSendToServer(int x, int y);

private slots:
    void present_yourself();
    // Слот чтения информации, получаемой от сервера
    void slotReadyRead();

    // Слот обработки ошибок сокета
    void slotError(QLocalSocket::LocalSocketError error);

    // Слот передачи информации на сервер
//    void slotSendToServer();

    // Слот обработки сигнала соединения с сервером
    void slotConnected();
    void slotSendToServer(QString command);
};

#endif
