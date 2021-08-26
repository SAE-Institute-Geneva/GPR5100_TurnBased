#pragma once

#include <vector>
#include <system.h>

#include <SFML/Graphics/RenderWindow.hpp>


class Engine
{
public:
    Engine();
    void AddSystem(System* system);
    void Run();
private:
    void Init();
    void Update(sf::Time dt);
    void Destroy();

    std::vector<System*> systems_;
    sf::RenderWindow window_; 
};