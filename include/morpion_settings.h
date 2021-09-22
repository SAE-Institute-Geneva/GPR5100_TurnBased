#pragma once

namespace morpion
{
constexpr int maxClientNmb = 2;
constexpr unsigned short serverPortNumber = 12345;
enum class MorpionPhase
{
    CONNECTION,
    GAME,
    END
};

using PlayerNumber = unsigned char;

}