// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CLIENT_H
#define CLIENT_H
//#include "window.h"
//#include "interactor.h"
#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>

//#ifndef WINDOW_H
//#include "window.h"
//#define WINDOW_H
//#endif
class Window;
class interactor;
class Client : public QWidget
{
    Q_OBJECT

public:
    friend class ConnectDialog;
    friend class Window;
    Client(QString serverName, int port, QDialog* dialog, QString NickName, QWidget* parent = 0);
    ~Client();
    Window* window;

//    void setInteractor(class interactor *interactor);
    void setPointer_to_UI(Window *window);
private:
//    interactor* interactor;
    QDialog* dialog;
    QString NickName;
    // Указатель на QTcpSocket
    QTcpSocket* localSocket;

    // Указатели на элементы интерфейса
    QTextEdit* textEdit;
    QPushButton* sendRevision;

    // Размер принимаемого от сервера блока
    quint16 nextBlockSize;

    void SendToServer(QString command);
//    void present_yourself();
    bool server_authenticate = false;
    QTimer* timer;
    void CallUpdateUI(QString message);
    void start_the_game();
    void CallUpdateTimeStamp(QString message);
public slots:
//    void slotSendToServer(int x, int y);

private slots:
    void present_yourself();
    // Слот чтения информации, получаемой от сервера
    void slotReadyRead();

    // Слот обработки ошибок сокета
    void slotError(QTcpSocket::SocketError error);

    // Слот передачи информации на сервер
//    void slotSendToServer();

    // Слот обработки сигнала соединения с сервером
    void slotConnected();
    void slotSendToServer(QString command);
};

#endif
