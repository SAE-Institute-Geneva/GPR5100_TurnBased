
#include "server.h"
#include "engine.h"

int main()
{
    Server server;
    Engine engine;
    engine.AddSystem(&server);
    engine.Run();
    return 0;
}
