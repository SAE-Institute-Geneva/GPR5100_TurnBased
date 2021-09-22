#pragma once

#include <string>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include "morpion_settings.h"
#include "system.h"

namespace morpion
{
class MorpionClient : public System
{
public:
    sf::Socket::Status Connect(sf::IpAddress address, unsigned short portNumber);
    MorpionPhase GetPhase() const;
    bool IsConnected() const;
    void Init() override;
    void ReceivePacket(sf::Packet& packet);
    void Update() override;
    void Destroy() override;
private:
    sf::TcpSocket socket_;
    MorpionPhase phase_ = MorpionPhase::CONNECTION;
};


class MorpionView : public DrawImGuiInterface
{
public:
    MorpionView(MorpionClient& client);
    void DrawImGui() override;
private:
    MorpionClient& client_;
    std::string ipAddressBuffer_ = "localhost";
    int portNumber_ = serverPortNumber;

};
}