#pragma once

#include <glm/vec4.hpp>

namespace zth::colors {

constexpr auto black = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
constexpr auto white = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };
constexpr auto red = glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
constexpr auto green = glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f };
constexpr auto blue = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };
constexpr auto yellow = glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f };
constexpr auto magenta = glm::vec4{ 1.0f, 0.0f, 1.0f, 1.0f };
constexpr auto cyan = glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f };
constexpr auto transparent = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

} // namespace zth::colors
