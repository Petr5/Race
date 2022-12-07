#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "window.h"
class interactor
{
public:
    interactor(Client* client);

    void start_the_game();
private:
    Window* window;
    Client* client;
};

#endif // INTERACTOR_H
