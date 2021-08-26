

#include "client.h"
#include "engine.h"

int main()
{
    Client client;
    Engine engine;
    engine.AddSystem(&client);
    engine.Run();
    return 0;
}
