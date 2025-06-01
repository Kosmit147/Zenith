#include "main_layer.hpp"
#include "main_scene.hpp"

auto main() -> int
{
    auto init_application_result = zth::Application::init();

    if (!init_application_result)
    {
        std::println(std::cerr, "CRITICAL ERROR: {}", init_application_result.error());
        return -1;
    }

    auto push_layer_result = zth::Application::push_layer(zth::make_unique<MainLayer>());

    if (!push_layer_result)
    {
        std::println(std::cerr, "CRITICAL ERROR: {}", push_layer_result.error());
        return -1;
    }

    zth::SceneManager::queue_scene<MainScene>();
    zth::Application::run();
}
