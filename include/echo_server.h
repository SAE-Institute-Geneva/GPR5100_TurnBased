#pragma once

#include <array>
#include <echo_settings.h>
#include <sfml/Network.hpp>


namespace echo
{

class EchoServer
{
public:
    int Run();
private:
    std::array<sf::TcpSocket, maxClientNmb> sockets_;
    sf::SocketSelector selector_;
    sf::TcpListener listener_;

    int GetNextSocket();
};

}