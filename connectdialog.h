#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "interactor.h"

#include "client.h"

class interactor;

class ConnectDialog: public QDialog
{
    Q_OBJECT
public:
    explicit ConnectDialog(QWidget *parent = nullptr);
private:
    Client* client;
    QGridLayout *mainLayout;
    QLineEdit* hostname;
    QLineEdit* port;
    QLineEdit* NickName;
    QPushButton* button;
private slots:
    void slotPushButtonClicked();

    void slotClientConnected();
};

#endif // CONNECTDIALOG_H
