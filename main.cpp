#include "window.h"
#include "client.h"
#include "connectdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectDialog dialog;
    dialog.show();
//    Client client(serverName, nullptr);
//    client.show();
//    Window window(&client);
//    window.show();
//    client.window = &window;

    return a.exec();
}
