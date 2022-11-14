#include "window.h"
#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString serverName("RaceServer");

    Client client(serverName);
    client.show();
//    Window window(&client);
//    window.show();
//    client.window = &window;

    return a.exec();
}
