#pragma once

#include <vector>
#include <system.h>

#include <SFML/Graphics/RenderWindow.hpp>


class Engine
{
public:
    Engine();
    void AddSystem(System* system);
    void AddDrawImGuiSystem(DrawImGuiInterface* drawImGuiSystem);
    void Run();
private:
    void Init();
    void Update(sf::Time dt);
    void Destroy();

    std::vector<System*> systems_;
    std::vector<DrawImGuiInterface*> drawImGuiSystems_;
    sf::RenderWindow window_; 
};