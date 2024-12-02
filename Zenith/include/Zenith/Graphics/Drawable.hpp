#pragma once

namespace zth {

class Drawable
{
public:
    virtual ~Drawable() = default;

    virtual auto draw() -> void = 0;
};

} // namespace zth
