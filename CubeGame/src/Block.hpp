#pragma once

#include <Zenith/Zenith.hpp>

class Block : public zth::CubeShape
{
public:
    explicit Block() = default;
    explicit Block(glm::ivec3 translation);
};
