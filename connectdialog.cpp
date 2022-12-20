#include "connectdialog.h"
#include "window.h"
#include <QMessageBox>

ConnectDialog::ConnectDialog(QWidget *parent): QDialog(parent)
{

//    setModal(true);
    mainLayout = new QGridLayout(this);
    hostname = new QLineEdit(this);
    port = new QLineEdit(this);
    NickName = new QLineEdit(this);
    button = new QPushButton(this);

    button->setText("Connect");
    hostname->setText("127.0.0.1");
    NickName->setText("Ivan");
    port->setText("3333");
    auto hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(hostname);
    auto portLabel = new QLabel(tr("S&erver port:"));
    portLabel->setBuddy(port);
    auto NickNameLabel = new QLabel(tr("NickName:"));
    NickNameLabel->setBuddy(NickName);

    mainLayout->addWidget(hostLabel,0,0);
    mainLayout->addWidget(hostname,0,1);
    mainLayout->addWidget(portLabel,1,0);
    mainLayout->addWidget(port,1,1);
    mainLayout->addWidget(NickNameLabel,2,0);
    mainLayout->addWidget(NickName,2,1);
    mainLayout->addWidget(button, 3, 1);

    connect(button, SIGNAL(clicked()), this, SLOT(slotPushButtonClicked()));
}

void ConnectDialog::slotPushButtonClicked(){

    client = new Client(hostname->text(),port->text().toInt(), this, NickName->text());
    client->NickName = NickName->text();
//    client->show();
    dialog = new QDialog();
    QGridLayout* layout = new QGridLayout();
//    QPushButton * button = new QPushButton(dialog);
//    button->setText(tr("ready"));
    auto infoLabel = new QLabel(tr("waiting for connection"));
//    infoLabel->setBuddy(button);
//    connect();
    layout->addWidget(infoLabel, 0, 0);
//    layout->addWidget(button, 0, 0);
    dialog->setLayout(layout);
    dialog->show();

//    interactor* interactor = new class interactor(client);
//    Window *window = new Window(client);
//    connect(client->localSocket, SIGNAL(connected()), this, SLOT(slotClientConnected()));

}

void ConnectDialog::hide_lobby(){
    dialog->hide();
}

void Client::start_the_game(){
    qInfo() << "called start the game";


    window = new Window(this);
    window->show();
    window->time_of_start_game = QDateTime::currentDateTime();
    dialog->hide_lobby();
}

void ConnectDialog::slotClientConnected(){
//    qInfo() << "try hide dialog and open client";
//    QMessageBox::information(this, "LogIn", "LogIn successfully");
//    qInfo() << "try to show client!!!!";
////    client->show();
////    client->setVisible(true);
    hide();

    dialog = new QDialog();
    QGridLayout* layout = new QGridLayout();
//    QPushButton * button = new QPushButton(dialog);
//    button->setText(tr("ready"));
    auto infoLabel = new QLabel(tr("waiting for connection"));
//    infoLabel->setBuddy(button);
//    connect();
    layout->addWidget(infoLabel, 0, 0);
//    layout->addWidget(button, 0, 0);
    dialog->setLayout(layout);
    dialog->show();

}
