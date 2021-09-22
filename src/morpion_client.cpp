#include "morpion_client.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>

#include "morpion_packet.h"

namespace morpion
{
sf::Socket::Status MorpionClient::Connect(sf::IpAddress address, unsigned short portNumber)
{
    const auto status = socket_.connect(address, portNumber);
    socket_.setBlocking(false);
    return status;
}

MorpionPhase MorpionClient::GetPhase() const
{
    return phase_;
}

bool MorpionClient::IsConnected() const
{
    return socket_.getLocalPort() != 0;
}

void MorpionClient::Init()
{
}

void MorpionClient::ReceivePacket(sf::Packet& packet)
{
    Packet morpionPacket{};
    packet >> morpionPacket;
    
    switch (static_cast<PacketType>(morpionPacket.packetType))
    {
    case PacketType::GAME_INIT:
    {
        GameInitPacket gameInitPacket{};
        packet >> gameInitPacket;
        std::cout << "You are player " << gameInitPacket.playerNumber + 1 << '\n';
        break;
    }
    default: 
        break;
    }
}

void MorpionClient::Update()
{
    //Receive packetS
    if(socket_.getLocalPort() != 0)
    {
        sf::Packet receivedPacket;
        sf::Socket::Status status;
        do
        {
            status = socket_.receive(receivedPacket);
        } while (status == sf::Socket::Partial);

        if (status == sf::Socket::Done)
        {
            ReceivePacket(receivedPacket);
        }

        if (status == sf::Socket::Disconnected)
        {
            socket_.disconnect();
            std::cerr << "Server disconnected\n";
        }
    }
}

void MorpionClient::Destroy()
{
}

MorpionView::MorpionView(MorpionClient& client) : client_(client)
{
}

void MorpionView::DrawImGui()
{
    if (client_.GetPhase() != MorpionPhase::CONNECTION || client_.IsConnected())
        return;

    ImGui::Begin("Client");
    
    ImGui::InputText("Ip Address", &ipAddressBuffer_);
    ImGui::InputInt("Port Number", &portNumber_);
    if (ImGui::Button("Connect"))
    {
        const auto status = client_.Connect(sf::IpAddress(ipAddressBuffer_), portNumber_);
        if (status != sf::Socket::Done)
        {
            switch (status)
            {
            case sf::Socket::NotReady:
                std::cerr << "Not ready to connect to " << ipAddressBuffer_ << ':' << portNumber_ << '\n';
                break;
            case sf::Socket::Partial:
                std::cerr << "Connecting to " << ipAddressBuffer_ << ':' << portNumber_ << '\n';
                break;
            case sf::Socket::Disconnected:
                std::cerr << "Disconnecting to " << ipAddressBuffer_ << ':' << portNumber_ << '\n';
                break;
            case sf::Socket::Error:
                std::cerr << "Error connecting to " << ipAddressBuffer_ << ':' << portNumber_ << '\n';
                break;
            default:;
            }
        }
        else
        {
            std::cout << "Successfully connected to server\n";
        }
    
    }
    ImGui::End();
}
}
