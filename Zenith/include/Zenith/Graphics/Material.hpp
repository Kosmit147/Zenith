#pragma once

#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

class Material
{
public:
    explicit Material(const Shader* shader = nullptr, const Texture2D* texture = nullptr);

    auto bind() const -> void;

    auto set_shader(const Shader* shader) -> void;
    auto set_texture(const Texture2D* texture) -> void;

    [[nodiscard]] auto shader() const { return _shader; }
    [[nodiscard]] auto texture() const { return _texture; }

private:
    const Shader* _shader = nullptr;
    const Texture2D* _texture = nullptr;
};

} // namespace zth
