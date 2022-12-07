#include "interactor.h"

interactor::interactor(Client* client)
{
    this->client = client;
//    client->setInteractor(this);

}

void interactor::start_the_game(){
   window = new Window(client);
    window->show();
}
