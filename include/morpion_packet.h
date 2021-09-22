#pragma once

#include <morpion_settings.h>
#include <SFML/Network/Packet.hpp>

namespace morpion
{
    enum class PacketType : unsigned char
    {
        GAME_INIT
    };

    struct Packet
    {
        unsigned char packetType;
    };

    sf::Packet& operator <<(sf::Packet& packet, const Packet& morpionPacket)
    {

        return packet << morpionPacket.packetType;
    }

    sf::Packet& operator >>(sf::Packet& packet, Packet& morpionPacket)
    {
        return packet >> morpionPacket.packetType ;
    }

    struct GameInitPacket : Packet
    {
        PlayerNumber playerNumber;
    };

    sf::Packet& operator <<(sf::Packet& packet, const GameInitPacket& gameInitPacket)
    {
        return packet << gameInitPacket.packetType
        << gameInitPacket.playerNumber;
    }

    sf::Packet& operator >>(sf::Packet& packet, GameInitPacket& gameInitPacket)
    {
        return packet >> gameInitPacket.playerNumber;
    }
}
