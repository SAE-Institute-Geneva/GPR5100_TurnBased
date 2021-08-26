#include <engine.h>
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Window/Event.hpp>

Engine::Engine() : window_(sf::VideoMode(640,480), "Turn Based")
{
}

void Engine::AddSystem(System* system)
{
    systems_.push_back(system);
}

void Engine::Run()
{
    Init();
    sf::Clock clock;
    while(window_.isOpen())
    {
        const auto dt = clock.restart();
        Update(dt);
    }
    Destroy();
}

void Engine::Init()
{
    for(auto* system: systems_)
    {
        system->Init();
    }
    window_.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window_);
}

void Engine::Update(sf::Time dt)
{
    sf::Event event{};
    while (window_.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        switch (event.type)
        {
        case sf::Event::Closed:
            window_.close();
            return;
        default:
            break;
        }
    }
    window_.clear(sf::Color::Black);
    for (auto* system : systems_)
    {
        system->Update();
    }
    ImGui::SFML::Update(window_, dt);

    for(auto* system : systems_)
    {
        system->DrawImGui();
    }
    ImGui::SFML::Render(window_);
    window_.display();
}

void Engine::Destroy()
{
    for (auto* system : systems_)
    {
        system->Destroy();
    }
    ImGui::SFML::Shutdown();
}
