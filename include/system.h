#pragma once

class System
{
public:
    virtual ~System() = default;
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;
};

class DrawImGuiInterface
{
public:
    virtual ~DrawImGuiInterface() = default;
    virtual void DrawImGui() = 0;
};