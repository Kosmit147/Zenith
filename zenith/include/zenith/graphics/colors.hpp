#pragma once

#include <glm/vec4.hpp>

namespace zth::colors {

constexpr inline auto black = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
constexpr inline auto white = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };
constexpr inline auto red = glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
constexpr inline auto green = glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f };
constexpr inline auto blue = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };
constexpr inline auto yellow = glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f };
constexpr inline auto magenta = glm::vec4{ 1.0f, 0.0f, 1.0f, 1.0f };
constexpr inline auto cyan = glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f };
constexpr inline auto transparent = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

} // namespace zth::colors
