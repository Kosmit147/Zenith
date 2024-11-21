#pragma once

#include <glad/glad.h>

#include "Zenith/Utility/Utility.hpp"

namespace zth {

class VertexArray
{
public:
    explicit VertexArray() { glGenVertexArrays(1, &_id); }
    ZTH_NO_COPY_NO_MOVE(VertexArray)
    ~VertexArray() { glDeleteVertexArrays(1, &_id); }

    auto bind() const -> void { glBindVertexArray(_id); }
    auto unbind() const -> void { glBindVertexArray(0); }

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id;
};

} // namespace zth
