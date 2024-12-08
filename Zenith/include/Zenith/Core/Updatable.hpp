#pragma once

namespace zth {

class Updatable
{
public:
    virtual ~Updatable() = default;

    virtual auto on_update() -> void = 0;
};

} // namespace zth
