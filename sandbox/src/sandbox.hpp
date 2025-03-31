#pragma once

class Sandbox : public zth::Application
{
public:
    explicit Sandbox();
    ZTH_NO_COPY_NO_MOVE(Sandbox)
    ~Sandbox() override = default;
};
