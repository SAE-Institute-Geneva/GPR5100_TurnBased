#pragma once

#include <system.h>
#include <array>
#include <echo_settings.h>
#include <string>
#include <sfml/Network/TcpSocket.hpp>


namespace echo
{

class EchoClient : public System, public DrawImGuiInterface
{
public:
    void Init() override;
    void Update() override;
    void Destroy() override;
    void DrawImGui() override;
private:
    sf::TcpSocket socket_;
    std::array<char, maxDataSize> msg_{};


    std::string ipAddressBuffer = "localhost";
    int portNumber = serverPortNumber;

    std::vector<std::string> receivedMsgs_;
};

}