#pragma once

class Testbed : public zth::Application
{
public:
    explicit Testbed();
    ZTH_NO_COPY_NO_MOVE(Testbed)
    ~Testbed() override = default;
};
