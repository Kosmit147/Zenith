#pragma once

#include <optional>

#include "Zenith/Core/ZthApi.hpp"
#include "Zenith/Platform/Window.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

struct ApplicationSpec
{
    WindowSpec window_spec{};
};

class ZTH_API Application
{
public:
    [[nodiscard]] static auto create(const ApplicationSpec& spec = {}) -> std::optional<Application>;

    ZTH_NO_COPY(Application)

    Application(Application&& other) noexcept;
    auto operator=(Application&& other) noexcept -> Application&;

    virtual ~Application() = default;

    auto run() const -> void;

private:
    Window _window;

private:
    explicit Application(Window&& window);

    // virtual auto on_update() -> void;
    // virtual auto on_event() -> void;
};

} // namespace zth
