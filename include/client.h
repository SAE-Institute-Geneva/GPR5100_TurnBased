#pragma once

#include <system.h>
#include <SFML/Network/TcpSocket.hpp>

class Client : public System
{
public:
    void Init() override;
    void Update() override;
    void Destroy() override;
    void DrawImGui() override;
private:
    sf::TcpSocket socket_;
};