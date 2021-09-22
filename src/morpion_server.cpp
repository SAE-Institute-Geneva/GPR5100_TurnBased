#include "morpion_server.h"
#include <SFML/Network/TcpSocket.hpp>

#include <iostream>
#include <random>

#include "morpion_packet.h"

namespace morpion
{
    void MorpionServer::ReceivePacket()
    {
        if (selector_.wait(sf::milliseconds(20)))
        {
            for (auto& socket : sockets_)
            {
                if (selector_.isReady(socket))
                {
                    sf::Packet receivedPacket;
                    sf::Socket::Status receivingStatus;
                    do
                    {
                        receivingStatus = socket.receive(receivedPacket);
                    } while (receivingStatus == sf::Socket::Partial);

                    Packet statusPacket;
                    receivedPacket >> statusPacket;
                    switch (static_cast<PacketType>(statusPacket.packetType))
                    {
                    case PacketType::MOVE:
                    {
                        MovePacket movePacket;
                        receivedPacket >> movePacket;
                        ManageMovePacket(movePacket);
                        break;
                    }
                    }

                }
            }
        }
    }

    void MorpionServer::ManageMovePacket(const MovePacket& movePacket)
    {
        std::cout << "Player " << movePacket.playerNumber + 1 <<
            " made move " << movePacket.position.x << ',' <<
            movePacket.position.y << '\n';

        if (phase_ != MorpionPhase::GAME)
            return;

        if(currentMoveIndex_ % 2 != movePacket.playerNumber)
        {
            //TODO return to player an error msg
            return;
        }

        for(unsigned char i = 0; i < currentMoveIndex_; i++)
        {
            if(moves_[i].position == movePacket.position)
                //TODO return an error msg
                return;
        }

        auto& currentMove = moves_[currentMoveIndex_];
        currentMove.position = movePacket.position;
        currentMove.playerNumber = movePacket.playerNumber;
        currentMoveIndex_++;
        MovePacket newMovePacket = movePacket;
        newMovePacket.packetType = static_cast<unsigned char>(PacketType::MOVE);

        //sent new move to all players
        for(auto& socket: sockets_)
        {
            sf::Packet sentPacket;
            sentPacket << newMovePacket;
            sf::Socket::Status sentStatus;
            do
            {
                sentStatus = socket.send(sentPacket);
            } while (sentStatus == sf::Socket::Partial);
        }
    }

    int MorpionServer::Run()
    {
        if (listener_.listen(serverPortNumber) != sf::Socket::Done)
        {
            std::cerr << "[Error] Server cannot bind port: " << serverPortNumber << '\n';
            return EXIT_FAILURE;
        }
        std::cout << "Server bound to port " << serverPortNumber << '\n';

        while (true)
        {
            ReceivePacket();
            switch (phase_)
            {
            case MorpionPhase::CONNECTION:
                UpdateConnectionPhase();
                break;
            case MorpionPhase::GAME:
                UpdateGamePhase();
                break;
            case MorpionPhase::END:
                UpdateEndPhase();
                break;
            default:;
            }
        }
    }

    void MorpionServer::StartNewGame()
    {
        //Switch to Game state
        phase_ = MorpionPhase::GAME;
        //Send game init packet
        std::cout << "Two players connected!\n";

        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 1);
        int dice_roll = distribution(generator);

        for (unsigned char i = 0; i < sockets_.size(); i++)
        {
            GameInitPacket gameInitPacket;
            gameInitPacket.packetType = static_cast<unsigned char>(PacketType::GAME_INIT);
            gameInitPacket.playerNumber = i != dice_roll;
            sf::Packet sentPacket;
            sentPacket << gameInitPacket;
            sf::Socket::Status sentStatus;
            do
            {
                sentStatus = sockets_[i].send(sentPacket);
            } while (sentStatus == sf::Socket::Partial);
        }
    }

    void MorpionServer::UpdateConnectionPhase()
    {
        // accept a new connection
        const auto nextIndex = GetNextSocket();

        if (nextIndex != -1)
        {
            auto& newSocket = sockets_[nextIndex];
            if (listener_.accept(newSocket) == sf::Socket::Done)
            {
                std::cout << "New connection from " <<
                    newSocket.getRemoteAddress().toString() << ':' << newSocket.
                    getRemotePort() << '\n';
                newSocket.setBlocking(false);
                selector_.add(newSocket);
                if (nextIndex == 1)
                {
                    StartNewGame();

                }
            }
        }
    }

    void MorpionServer::UpdateGamePhase()
    {
    }

    void MorpionServer::UpdateEndPhase()
    {
    }

    int MorpionServer::GetNextSocket()
    {
        for (int i = 0; i < maxClientNmb; i++)
        {
            if (sockets_[i].getLocalPort() == 0)
            {
                return i;
            }
        }
        return -1;
    }
}
