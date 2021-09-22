#pragma once

#include <array>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <morpion_settings.h>

namespace morpion
{

class MorpionServer
{
public:
    int Run();
private:
    std::array<sf::TcpSocket, maxClientNmb> sockets_;
    sf::SocketSelector selector_;
    sf::TcpListener listener_;
    MorpionPhase phase_ = MorpionPhase::CONNECTION;

    void StartNewGame();
    void UpdateConnectionPhase();
    void UpdateGamePhase();
    void UpdateEndPhase();

    int GetNextSocket();
};
}
