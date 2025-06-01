#include "zenith/debug/ui.hpp"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <imgui_stdlib.h>

#include "zenith/core/assert.hpp"
#include "zenith/core/scene.hpp"
#include "zenith/ecs/components.hpp"
#include "zenith/gl/context.hpp"
#include "zenith/memory/memory.hpp"
#include "zenith/renderer/light.hpp"
#include "zenith/renderer/renderer.hpp"
#include "zenith/stl/string_algorithm.hpp"
#include "zenith/system/application.hpp"
#include "zenith/system/input.hpp"
#include "zenith/system/temporary_storage.hpp"
#include "zenith/system/window.hpp"

namespace zth::debug {

namespace {

constexpr auto far_plane_drag_speed = default_float_drag_speed * 10.0f;
constexpr auto light_attenuation_drag_speed = default_float_drag_speed * 0.01f;
constexpr auto light_ambient_drag_speed = default_float_drag_speed * 0.1f;
constexpr auto material_shininess_drag_speed = default_float_drag_speed * 10.0f;

constexpr auto default_relative_item_width = 12.0f;

auto select_stringifiable_enum(const char* label, auto& value, const auto& enum_values) -> bool
{
    auto value_changed = false;

    if (ImGui::BeginCombo(label, to_string(value)))
    {
        for (auto current_value : enum_values)
        {
            const auto is_selected = current_value == value;

            if (ImGui::Selectable(to_string(current_value), is_selected))
            {
                value = current_value;
                value_changed = true;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    return value_changed;
}

template<typename Component> auto display_component_for_entity_in_inspector(EntityHandle entity) -> void
{
    ZTH_ASSERT(entity.any_of<Component>());

    if (ImGui::TreeNodeEx(Component::display_label(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto remove_component = false;

        if constexpr (!IntegralComponent<Component>)
        {
            // Component is removable.

            if (ImGui::BeginPopupContextItem(nullptr, ImGuiPopupFlags_MouseButtonRight))
            {
                if (ImGui::MenuItem("Remove Component"))
                    remove_component = true;

                ImGui::EndPopup();
            }
        }

        auto& component = entity.get<Component>();
        edit_component(component);

        if constexpr (!IntegralComponent<Component>)
        {
            // Component is removable.

            if (remove_component)
                entity.erase<Component>();
        }

        ImGui::TreePop();
    }
}

} // namespace

auto begin_window(const char* label) -> void
{
    ImGui::Begin(label);
}

auto end_window() -> void
{
    ImGui::End();
}

auto text(const char* txt) -> void
{
    ImGui::TextUnformatted(txt);
}

auto text(StringView txt) -> void
{
    ImGui::TextUnformatted(txt.data(), txt.data() + txt.size());
}

auto separator_text(const char* txt) -> void
{
    ImGui::SeparatorText(txt);
}

auto text_wrapped(const char* txt) -> void
{
    ImGui::PushTextWrapPos(0.0f);
    text(txt);
    ImGui::PopTextWrapPos();
}

auto text_wrapped(StringView txt) -> void
{
    ImGui::PushTextWrapPos(0.0f);
    text(txt);
    ImGui::PopTextWrapPos();
}

auto drag_int(const char* label, u8& value, float drag_speed) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_U8, &value, drag_speed);
}

auto drag_int_2(const char* label, u8 values[2], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U8, values, 2, drag_speed);
}

auto drag_int_3(const char* label, u8 values[3], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U8, values, 3, drag_speed);
}

auto drag_int_4(const char* label, u8 values[4], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U8, values, 4, drag_speed);
}

auto drag_int(const char* label, i8& value, float drag_speed) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_S8, &value, drag_speed);
}

auto drag_int_2(const char* label, i8 values[2], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S8, values, 2, drag_speed);
}

auto drag_int_3(const char* label, i8 values[3], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S8, values, 3, drag_speed);
}

auto drag_int_4(const char* label, i8 values[4], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S8, values, 4, drag_speed);
}

auto drag_int(const char* label, u16& value, float drag_speed) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_U16, &value, drag_speed);
}

auto drag_int_2(const char* label, u16 values[2], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U16, values, 2, drag_speed);
}

auto drag_int_3(const char* label, u16 values[3], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U16, values, 3, drag_speed);
}

auto drag_int_4(const char* label, u16 values[4], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U16, values, 4, drag_speed);
}

auto drag_int(const char* label, i16& value, float drag_speed) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_S16, &value, drag_speed);
}

auto drag_int_2(const char* label, i16 values[2], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S16, values, 2, drag_speed);
}

auto drag_int_3(const char* label, i16 values[3], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S16, values, 3, drag_speed);
}

auto drag_int_4(const char* label, i16 values[4], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S16, values, 4, drag_speed);
}

auto drag_int(const char* label, u32& value, float drag_speed) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_U32, &value, drag_speed);
}

auto drag_int_2(const char* label, u32 values[2], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U32, values, 2, drag_speed);
}

auto drag_int_3(const char* label, u32 values[3], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U32, values, 3, drag_speed);
}

auto drag_int_4(const char* label, u32 values[4], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U32, values, 4, drag_speed);
}

auto drag_int(const char* label, i32& value, float drag_speed) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_S32, &value, drag_speed);
}

auto drag_int_2(const char* label, i32 values[2], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S32, values, 2, drag_speed);
}

auto drag_int_3(const char* label, i32 values[3], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S32, values, 3, drag_speed);
}

auto drag_int_4(const char* label, i32 values[4], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S32, values, 4, drag_speed);
}

auto drag_int(const char* label, u64& value, float drag_speed) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_U64, &value, drag_speed);
}

auto drag_int_2(const char* label, u64 values[2], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U64, values, 2, drag_speed);
}

auto drag_int_3(const char* label, u64 values[3], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U64, values, 3, drag_speed);
}

auto drag_int_4(const char* label, u64 values[4], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_U64, values, 4, drag_speed);
}

auto drag_int(const char* label, i64& value, float drag_speed) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_S64, &value, drag_speed);
}

auto drag_int_2(const char* label, i64 values[2], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S64, values, 2, drag_speed);
}

auto drag_int_3(const char* label, i64 values[3], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S64, values, 3, drag_speed);
}

auto drag_int_4(const char* label, i64 values[4], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_S64, values, 4, drag_speed);
}

auto drag_float(const char* label, float& value, float drag_speed) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_Float, &value, drag_speed);
}

auto drag_float_2(const char* label, float values[2], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_Float, values, 2, drag_speed);
}

auto drag_float_3(const char* label, float values[3], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_Float, values, 3, drag_speed);
}

auto drag_float_4(const char* label, float values[4], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_Float, values, 4, drag_speed);
}

auto drag_float(const char* label, double& value, float drag_speed) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_Double, &value, drag_speed);
}

auto drag_float_2(const char* label, double values[2], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_Double, values, 2, drag_speed);
}

auto drag_float_3(const char* label, double values[3], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_Double, values, 3, drag_speed);
}

auto drag_float_4(const char* label, double values[4], float drag_speed) -> bool
{
    return ImGui::DragScalarN(label, ImGuiDataType_Double, values, 4, drag_speed);
}

auto drag_vec(const char* label, glm::uvec2& vec, float drag_speed) -> bool
{
    return drag_int_2(label, glm::value_ptr(vec), drag_speed);
}

auto drag_vec(const char* label, glm::uvec3& vec, float drag_speed) -> bool
{
    return drag_int_3(label, glm::value_ptr(vec), drag_speed);
}

auto drag_vec(const char* label, glm::uvec4& vec, float drag_speed) -> bool
{
    return drag_int_4(label, glm::value_ptr(vec), drag_speed);
}

auto drag_vec(const char* label, glm::ivec2& vec, float drag_speed) -> bool
{
    return drag_int_2(label, glm::value_ptr(vec), drag_speed);
}

auto drag_vec(const char* label, glm::ivec3& vec, float drag_speed) -> bool
{
    return drag_int_3(label, glm::value_ptr(vec), drag_speed);
}

auto drag_vec(const char* label, glm::ivec4& vec, float drag_speed) -> bool
{
    return drag_int_4(label, glm::value_ptr(vec), drag_speed);
}

auto drag_vec(const char* label, glm::vec2& vec, float drag_speed) -> bool
{
    return drag_float_2(label, glm::value_ptr(vec), drag_speed);
}

auto drag_vec(const char* label, glm::vec3& vec, float drag_speed) -> bool
{
    return drag_float_3(label, glm::value_ptr(vec), drag_speed);
}

auto drag_vec(const char* label, glm::vec4& vec, float drag_speed) -> bool
{
    return drag_float_4(label, glm::value_ptr(vec), drag_speed);
}

auto drag_angles(const char* label, glm::vec2& angles, float drag_speed) -> bool
{
    auto degrees = glm::degrees(angles);

    if (drag_float_2(label, glm::value_ptr(degrees), drag_speed))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto drag_angles(const char* label, glm::vec3& angles, float drag_speed) -> bool
{
    auto degrees = glm::degrees(angles);

    if (drag_float_3(label, glm::value_ptr(degrees), drag_speed))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto drag_angles(const char* label, glm::vec4& angles, float drag_speed) -> bool
{
    auto degrees = glm::degrees(angles);

    if (drag_float_4(label, glm::value_ptr(degrees), drag_speed))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto slide_int(const char* label, u8& value, u8 min, u8 max) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_U8, &value, &min, &max);
}

auto slide_int_2(const char* label, u8 values[2], u8 min, u8 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U8, values, 2, &min, &max);
}

auto slide_int_3(const char* label, u8 values[3], u8 min, u8 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U8, values, 3, &min, &max);
}

auto slide_int_4(const char* label, u8 values[4], u8 min, u8 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U8, values, 4, &min, &max);
}

auto slide_int(const char* label, i8& value, i8 min, i8 max) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_S8, &value, &min, &max);
}

auto slide_int_2(const char* label, i8 values[2], i8 min, i8 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S8, values, 2, &min, &max);
}

auto slide_int_3(const char* label, i8 values[3], i8 min, i8 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S8, values, 3, &min, &max);
}

auto slide_int_4(const char* label, i8 values[4], i8 min, i8 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S8, values, 4, &min, &max);
}

auto slide_int(const char* label, u16& value, u16 min, u16 max) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_U16, &value, &min, &max);
}

auto slide_int_2(const char* label, u16 values[2], u16 min, u16 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U16, values, 2, &min, &max);
}

auto slide_int_3(const char* label, u16 values[3], u16 min, u16 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U16, values, 3, &min, &max);
}

auto slide_int_4(const char* label, u16 values[4], u16 min, u16 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U16, values, 4, &min, &max);
}

auto slide_int(const char* label, i16& value, i16 min, i16 max) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_S16, &value, &min, &max);
}

auto slide_int_2(const char* label, i16 values[2], i16 min, i16 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S16, values, 2, &min, &max);
}

auto slide_int_3(const char* label, i16 values[3], i16 min, i16 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S16, values, 3, &min, &max);
}

auto slide_int_4(const char* label, i16 values[4], i16 min, i16 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S16, values, 4, &min, &max);
}

auto slide_int(const char* label, u32& value, u32 min, u32 max) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_U32, &value, &min, &max);
}

auto slide_int_2(const char* label, u32 values[2], u32 min, u32 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U32, values, 2, &min, &max);
}

auto slide_int_3(const char* label, u32 values[3], u32 min, u32 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U32, values, 3, &min, &max);
}

auto slide_int_4(const char* label, u32 values[4], u32 min, u32 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U32, values, 4, &min, &max);
}

auto slide_int(const char* label, i32& value, i32 min, i32 max) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_S32, &value, &min, &max);
}

auto slide_int_2(const char* label, i32 values[2], i32 min, i32 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S32, values, 2, &min, &max);
}

auto slide_int_3(const char* label, i32 values[3], i32 min, i32 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S32, values, 3, &min, &max);
}

auto slide_int_4(const char* label, i32 values[4], i32 min, i32 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S32, values, 4, &min, &max);
}

auto slide_int(const char* label, u64& value, u64 min, u64 max) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_U64, &value, &min, &max);
}

auto slide_int_2(const char* label, u64 values[2], u64 min, u64 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U64, values, 2, &min, &max);
}

auto slide_int_3(const char* label, u64 values[3], u64 min, u64 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U64, values, 3, &min, &max);
}

auto slide_int_4(const char* label, u64 values[4], u64 min, u64 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_U64, values, 4, &min, &max);
}

auto slide_int(const char* label, i64& value, i64 min, i64 max) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_S64, &value, &min, &max);
}

auto slide_int_2(const char* label, i64 values[2], i64 min, i64 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S64, values, 2, &min, &max);
}

auto slide_int_3(const char* label, i64 values[3], i64 min, i64 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S64, values, 3, &min, &max);
}

auto slide_int_4(const char* label, i64 values[4], i64 min, i64 max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_S64, values, 4, &min, &max);
}

auto slide_float(const char* label, float& value, float min, float max) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_Float, &value, &min, &max);
}

auto slide_float_2(const char* label, float values[2], float min, float max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_Float, values, 2, &min, &max);
}

auto slide_float_3(const char* label, float values[3], float min, float max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_Float, values, 3, &min, &max);
}

auto slide_float_4(const char* label, float values[4], float min, float max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_Float, values, 4, &min, &max);
}

auto slide_float(const char* label, double& value, double min, double max) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_Double, &value, &min, &max);
}

auto slide_float_2(const char* label, double values[2], double min, double max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_Double, values, 2, &min, &max);
}

auto slide_float_3(const char* label, double values[3], double min, double max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_Double, values, 3, &min, &max);
}

auto slide_float_4(const char* label, double values[4], double min, double max) -> bool
{
    return ImGui::SliderScalarN(label, ImGuiDataType_Double, values, 4, &min, &max);
}

auto slide_vec(const char* label, glm::uvec2& vec, u32 min, u32 max) -> bool
{
    return slide_int_2(label, glm::value_ptr(vec), min, max);
}

auto slide_vec(const char* label, glm::uvec3& vec, u32 min, u32 max) -> bool
{
    return slide_int_3(label, glm::value_ptr(vec), min, max);
}

auto slide_vec(const char* label, glm::uvec4& vec, u32 min, u32 max) -> bool
{
    return slide_int_4(label, glm::value_ptr(vec), min, max);
}

auto slide_vec(const char* label, glm::vec2& vec, float min, float max) -> bool
{
    return slide_float_2(label, glm::value_ptr(vec), min, max);
}

auto slide_vec(const char* label, glm::vec3& vec, float min, float max) -> bool
{
    return slide_float_3(label, glm::value_ptr(vec), min, max);
}

auto slide_vec(const char* label, glm::vec4& vec, float min, float max) -> bool
{
    return slide_float_4(label, glm::value_ptr(vec), min, max);
}

auto slide_angles(const char* label, glm::vec2& angles, float min_degrees, float max_degrees) -> bool
{
    auto degrees = glm::degrees(angles);

    if (slide_float_2(label, glm::value_ptr(degrees), min_degrees, max_degrees))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto slide_angles(const char* label, glm::vec3& angles, float min_degrees, float max_degrees) -> bool
{
    auto degrees = glm::degrees(angles);

    if (slide_float_3(label, glm::value_ptr(degrees), min_degrees, max_degrees))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto slide_angles(const char* label, glm::vec4& angles, float min_degrees, float max_degrees) -> bool
{
    auto degrees = glm::degrees(angles);

    if (slide_float_4(label, glm::value_ptr(degrees), min_degrees, max_degrees))
    {
        angles = glm::radians(degrees);
        return true;
    }

    return false;
}

auto checkbox(const char* label, bool& value) -> bool
{
    return ImGui::Checkbox(label, &value);
}

auto button(const char* label) -> bool
{
    return ImGui::Button(label);
}

auto edit_color(const char* label, glm::vec3& color) -> bool
{
    return ImGui::ColorEdit3(label, glm::value_ptr(color));
}

auto edit_color(const char* label, glm::vec4& color) -> bool
{
    return ImGui::ColorEdit4(label, glm::value_ptr(color));
}

auto pick_color(const char* label, glm::vec3& color) -> bool
{
    return ImGui::ColorPicker3(label, glm::value_ptr(color));
}

auto pick_color(const char* label, glm::vec4& color) -> bool
{
    return ImGui::ColorPicker4(label, glm::value_ptr(color));
}

auto input_int(const char* label, u32& value, u32 step) -> bool
{
    return ImGui::InputScalar(label, ImGuiDataType_U32, &value, step != 0 ? &step : nullptr);
}

auto input_int(const char* label, i32& value, i32 step) -> bool
{
    return ImGui::InputScalar(label, ImGuiDataType_S32, &value, step != 0 ? &step : nullptr);
}

auto input_int(const char* label, u64& value, u64 step) -> bool
{
    return ImGui::InputScalar(label, ImGuiDataType_U64, &value, step != 0 ? &step : nullptr);
}

auto input_int(const char* label, i64& value, i64 step) -> bool
{
    return ImGui::InputScalar(label, ImGuiDataType_S64, &value, step != 0 ? &step : nullptr);
}

auto input_float(const char* label, float& value, float step) -> bool
{
    return ImGui::InputScalar(label, ImGuiDataType_Float, &value, step != 0.0f ? &step : nullptr);
}

auto input_float(const char* label, double& value, double step) -> bool
{
    return ImGui::InputScalar(label, ImGuiDataType_Double, &value, step != 0.0 ? &step : nullptr);
}

auto input_text(const char* label, String& value) -> bool
{
    return ImGui::InputText(label, &value);
}

auto drag_rect(const char* label, Rect<u32>& rect, float drag_speed) -> bool
{
    text(label);
    auto value_changed = false;
    value_changed |= drag_vec("Position", rect.position, drag_speed);
    value_changed |= drag_vec("Size", rect.size, drag_speed);
    return value_changed;
}

auto drag_rect(const char* label, BoundedRect<u32>& rect, float drag_speed) -> bool
{
    text(label);
    auto value_changed = false;
    value_changed |= drag_vec("Top-left", rect.top_left, drag_speed);
    value_changed |= drag_vec("Bottom-right", rect.bottom_right, drag_speed);
    return value_changed;
}

auto slide_rect(const char* label, Rect<u32>& rect, Rect<u32> min, Rect<u32> max) -> bool
{
    text(label);
    auto value_changed = false;
    value_changed |= slide_int("Position (X):", rect.position.x, min.position.x, max.position.x);
    value_changed |= slide_int("Position (Y):", rect.position.y, min.position.y, max.position.y);
    value_changed |= slide_int("Size (X):", rect.size.x, min.size.x, max.size.x);
    value_changed |= slide_int("Size (Y):", rect.size.y, min.size.y, max.size.y);
    return value_changed;
}

auto slide_rect(const char* label, BoundedRect<u32>& rect, BoundedRect<u32> min, BoundedRect<u32> max) -> bool
{
    text(label);
    auto value_changed = false;
    value_changed |= slide_int("Top-left (X):", rect.top_left.x, min.top_left.x, max.top_left.x);
    value_changed |= slide_int("Top-left (Y):", rect.top_left.y, min.top_left.y, max.top_left.y);
    value_changed |= slide_int("Bottom-right (X):", rect.bottom_right.x, min.bottom_right.x, max.bottom_right.x);
    value_changed |= slide_int("Bottom-right (Y):", rect.bottom_right.y, min.bottom_right.y, max.bottom_right.y);
    return value_changed;
}

auto slide_angle(const char* label, float& angle_radians, float degrees_min, float degrees_max) -> bool
{
    return ImGui::SliderAngle(label, &angle_radians, degrees_min, degrees_max);
}

auto drag_euler_angles(const char* label, math::EulerAngles& angles, float drag_speed) -> bool
{
    auto values = static_cast<glm::vec3>(angles);

    if (drag_angles(label, values, drag_speed))
    {
        angles = math::EulerAngles{ .pitch = values.x, .yaw = values.y, .roll = values.z };
        return true;
    }

    return false;
}

auto edit_quat(const char* label, glm::quat& quaternion) -> bool
{
    return slide_float_4(label, glm::value_ptr(quaternion));
}

auto edit_quat_as_euler_angles(const char* label, glm::quat& quaternion) -> bool
{
    const auto original_angles = math::to_euler_angles(quaternion);
    auto new_angles = original_angles;

    if (drag_euler_angles(label, new_angles))
    {
        auto change = new_angles - original_angles;
        quaternion = math::rotate(quaternion, change);
        return true;
    }

    return false;
}

auto select_key(const char* label, Key& key) -> bool
{
    return select_stringifiable_enum(label, key, key_enumerations);
}

auto select_mouse_button(const char* label, MouseButton& button) -> bool
{
    return select_stringifiable_enum(label, button, mouse_button_enumerations);
}

auto select_light_type(LightType& type) -> bool
{
    return select_stringifiable_enum("Type", type, light_type_enumerations);
}

auto edit_light_properties(LightProperties& properties) -> void
{
    edit_color("Color", properties.color);
    drag_vec("Ambient", properties.ambient, light_ambient_drag_speed);
    drag_vec("Diffuse", properties.diffuse);
    drag_vec("Specular", properties.specular);
}

auto edit_light_attenuation(LightAttenuation& attenuation) -> void
{
    drag_float("Constant", attenuation.constant, light_attenuation_drag_speed);
    drag_float("Linear", attenuation.linear, light_attenuation_drag_speed);
    drag_float("Quadratic", attenuation.quadratic, light_attenuation_drag_speed);
}

auto edit_directional_light(DirectionalLight& light) -> void
{
    edit_light_properties(light.properties);
}

auto edit_point_light(PointLight& light) -> void
{
    edit_light_properties(light.properties);
    edit_light_attenuation(light.attenuation);
}

auto edit_spot_light(SpotLight& light) -> void
{
    float inner_cutoff = glm::acos(light.inner_cutoff_cosine);
    float outer_cutoff = glm::acos(light.outer_cutoff_cosine);

    if (slide_angle("Inner Cutoff", inner_cutoff))
        light.inner_cutoff_cosine = glm::cos(inner_cutoff);

    if (slide_angle("Outer Cutoff", outer_cutoff))
        light.outer_cutoff_cosine = glm::cos(outer_cutoff);

    edit_light_properties(light.properties);
    edit_light_attenuation(light.attenuation);
}

auto edit_ambient_light(AmbientLight& light) -> void
{
    drag_vec("Ambient", light.ambient, light_ambient_drag_speed);
}

auto edit_component(TagComponent& tag) -> void
{
    input_text(TagComponent::display_label(), tag.tag);
}

auto edit_component(TransformComponent& transform) -> void
{
    auto translation = transform.translation();

    if (drag_vec("Translation", translation))
        transform.set_translation(translation);

    auto rotation = transform.rotation();

    if (edit_quat_as_euler_angles("Rotation", rotation))
        transform.set_rotation(rotation);

    auto scale = transform.scale();

    if (drag_vec("Scale", scale))
        transform.set_scale(scale);
}

auto edit_component(CameraComponent& camera) -> void
{
    drag_float("Aspect Ratio", camera.aspect_ratio);
    slide_angle("FOV", camera.fov, 0.0f, 180.0f);
    drag_float("Near Plane", camera.near);
    drag_float("Far Plane", camera.far, far_plane_drag_speed);
}

auto edit_component(LightComponent& light) -> void
{
    auto light_type = light.type();

    if (select_light_type(light_type))
        light.set_light(light_type);

    switch (light.type())
    {
        using enum LightType;
    case Directional:
        edit_directional_light(light.directional_light());
        break;
    case Point:
        edit_point_light(light.point_light());
        break;
    case Spot:
        edit_spot_light(light.spot_light());
        break;
    case Ambient:
        edit_ambient_light(light.ambient_light());
        break;
    }
}

auto edit_component(SpriteRenderer2DComponent& sprite) -> void
{
    // @todo: Edit texture.

    drag_rect("Rect", sprite.rect);
    edit_color("Color", sprite.color);
}

auto edit_component(MeshRendererComponent& mesh) -> void
{
    (void)mesh;

    text("TODO!");

    // @todo
}

auto edit_component(MaterialComponent& material) -> void
{
    (void)material;

    text("TODO!");

    // @todo
}

auto edit_component(ScriptComponent& script) -> void
{
    text(script.script().display_label());
    script.script().debug_edit();
}

auto TransformGizmo::manipulate(TransformComponent& transform) const -> bool
{
    ImGuizmo::Enable(true);
    auto transform_matrix = transform.transform();

    auto& view = Renderer::current_camera_view();
    auto& projection = Renderer::current_camera_projection();

    auto gizmo_operation = [this] {
        switch (operation)
        {
            using enum GizmoOperation;
        case Translate:
            return ImGuizmo::TRANSLATE;
        case Rotate:
            return ImGuizmo::ROTATE;
        case Scale:
            return ImGuizmo::SCALE;
        }

        ZTH_ASSERT(false);
        return ImGuizmo::TRANSLATE;
    }();

    auto gizmo_mode = [this] {
        switch (mode)
        {
            using enum GizmoMode;
        case Local:
            return ImGuizmo::LOCAL;
        case World:
            return ImGuizmo::WORLD;
        }

        ZTH_ASSERT(false);
        return ImGuizmo::LOCAL;
    }();

    if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), gizmo_operation, gizmo_mode,
                             glm::value_ptr(transform_matrix), nullptr, nullptr))
    {
        transform.set_transform(transform_matrix);
        return true;
    }

    return false;
}

auto EntityInspectorPanel::display(EntityHandle entity) const -> void
{
    ZTH_ASSERT(entity.valid());

    begin_window("Entity Inspector");
    ImGui::PushItemWidth(ImGui::GetFontSize() * default_relative_item_width);

    {
        edit_component(entity.tag());
        text("ID: {}", entity.id());
        display_component_for_entity_in_inspector<TransformComponent>(entity);

        if (Window::cursor_enabled())
            gizmo.manipulate(entity.transform());
    }

    if (entity.any_of<CameraComponent>())
        display_component_for_entity_in_inspector<CameraComponent>(entity);

    if (entity.any_of<LightComponent>())
        display_component_for_entity_in_inspector<LightComponent>(entity);

    if (entity.any_of<SpriteRenderer2DComponent>())
        display_component_for_entity_in_inspector<SpriteRenderer2DComponent>(entity);

    if (entity.any_of<MeshRendererComponent>())
        display_component_for_entity_in_inspector<MeshRendererComponent>(entity);

    if (entity.any_of<MaterialComponent>())
        display_component_for_entity_in_inspector<MaterialComponent>(entity);

    if (entity.any_of<ScriptComponent>())
        display_component_for_entity_in_inspector<ScriptComponent>(entity);

    separator_text("##");

    if (button("Add Component"))
        ImGui::OpenPopup("AddComponentPopup");

    if (ImGui::BeginPopup("AddComponentPopup"))
    {
        auto add_component_menu_item = [&]<typename Component>(std::type_identity<Component>) {
            if (ImGui::MenuItem(Component::display_label()))
            {
                entity.try_emplace<Component>();
                ImGui::CloseCurrentPopup();
            }
        };

        add_component_menu_item(std::type_identity<CameraComponent>{});
        add_component_menu_item(std::type_identity<LightComponent>{});
        add_component_menu_item(std::type_identity<SpriteRenderer2DComponent>{});
        add_component_menu_item(std::type_identity<MeshRendererComponent>{});
        add_component_menu_item(std::type_identity<MaterialComponent>{});
        add_component_menu_item(std::type_identity<ScriptComponent>{});

        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();
    end_window();
}

auto SceneHierarchyPanel::display(Registry& registry) -> void
{
    begin_window("Scene Hierarchy");

    input_text("Search", _search);

    for (auto entity_id : registry.view<EntityId>())
    {
        const auto& tag = registry.get<const TagComponent>(entity_id);

        if (!_search.empty())
        {
            if (!case_insensitive_contains(tag.tag, _search))
                continue;
        }

        auto label = format_to_temporary("{}##{}", tag.tag, entity_id);

        if (ImGui::Selectable(label.c_str(), _selected_entity_id == entity_id))
            _selected_entity_id = entity_id;

        if (ImGui::BeginPopupContextItem(nullptr, ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("Delete Entity"))
                registry.destroy(entity_id);

            ImGui::EndPopup();
        }
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
        _selected_entity_id = null_entity;
    else if (registry.valid(_selected_entity_id))
        inspector.display(EntityHandle{ _selected_entity_id, registry });

    if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
        if (ImGui::MenuItem("Create Entity"))
            registry.create();

        ImGui::EndPopup();
    }

    end_window();
}

DebugPanel::DebugPanel(StringView label) : _label{ label } {}

auto DebugPanel::display() -> void
{
    begin_window(_label.c_str());

    {
        text("FPS: {:.2f}", Application::frame_rate());

        auto frame_time = Application::frame_time();
        auto fixed_update_time = Application::fixed_update_time();
        auto update_time = Application::update_time();
        auto render_time = Application::render_time();

        text("Frame time: {:.4f}ms", frame_time * 1000.0);
        text("Fixed update time: {:.4f}ms ({:.2f}%)", fixed_update_time * 1000.0,
             fixed_update_time / frame_time * 100.0);
        text("Update time: {:.4f}ms ({:.2f}%)", update_time * 1000.0, update_time / frame_time * 100.0);
        text("Render time: {:.4f}ms ({:.2f}%)", render_time * 1000.0, render_time / frame_time * 100.0);

        text("Draw Calls (3D): {}", Renderer::draw_calls_last_frame());
        text("Draw Calls (2D): {}", Renderer2D::draw_calls_last_frame());

        auto temporary_storage_capacity = TemporaryStorage::capacity();
        auto temporary_storage_usage = TemporaryStorage::usage_last_frame();

        // @todo: We should choose the unit to use here dynamically instead of always using MB.
        text("Temporary storage capacity: {:.2f}MB", memory::to_megabytes(temporary_storage_capacity));
        text("Temporary storage usage: {:.0f}%",
             static_cast<double>(temporary_storage_usage) / static_cast<double>(temporary_storage_capacity));
    }

    bool frame_rate_limit_enabled;

    {
        auto limit = Window::frame_rate_limit();
        frame_rate_limit_enabled = limit.has_value();
        _frame_rate_limit = limit.value_or(_frame_rate_limit);
    }

    if (checkbox("FPS Limit", frame_rate_limit_enabled))
    {
        if (frame_rate_limit_enabled)
            Window::set_frame_rate_limit(_frame_rate_limit);
        else
            Window::disable_frame_rate_limit();
    }

    if (frame_rate_limit_enabled)
    {
        if (input_int("##", _frame_rate_limit))
            Window::set_frame_rate_limit(_frame_rate_limit);
    }

    {
        auto blending_enabled = Renderer::blending_enabled();

        if (checkbox("Blending", blending_enabled))
            Renderer::set_blending_enabled(blending_enabled);
    }

    {
        auto depth_test_enabled = Renderer::depth_test_enabled();

        if (checkbox("Depth Test", depth_test_enabled))
            Renderer::set_depth_test_enabled(depth_test_enabled);
    }

    {
        auto face_culling_enabled = Renderer::face_culling_enabled();

        if (checkbox("Face Culling", face_culling_enabled))
            Renderer::set_face_culling_enabled(face_culling_enabled);
    }

    {
        auto multisampling_enabled = Renderer::multisampling_enabled();

        if (checkbox("Multisampling", multisampling_enabled))
            Renderer::set_multisampling_enabled(multisampling_enabled);
    }

    {
        auto wireframe_mode_enabled = Renderer::wireframe_mode_enabled();

        if (checkbox("Wireframe", wireframe_mode_enabled))
            Renderer::set_wireframe_mode_enabled(wireframe_mode_enabled);
    }

    input_float("Delta time limit", Application::delta_time_limit);
    input_float("Fixed time step", Application::fixed_time_step);

    text("Vendor: {}", gl::Context::vendor_string());
    text("Renderer: {}", gl::Context::renderer_string());
    text("Version: {}", gl::Context::version_string());
    text("GLSL Version: {}", gl::Context::glsl_version_string());

    end_window();
}

ScenePicker::ScenePicker(StringView label) : _label{ label } {}

auto ScenePicker::display() -> void
{
    begin_window(_label.c_str());

    text("{}", _scene_names[_selected_scene_idx]);
    text("Prev");

    ImGui::SameLine();

    if (ImGui::ArrowButton("Prev", ImGuiDir_Left))
        prev_scene();

    ImGui::SameLine();

    if (ImGui::ArrowButton("Next", ImGuiDir_Right))
        next_scene();

    ImGui::SameLine();

    text("Next");

    end_window();
}

auto ScenePicker::prev_scene() -> void
{
    if (_scene_count == 0)
        return;

    if (_selected_scene_idx == 0)
        _selected_scene_idx = _scene_count - 1;
    else
        _selected_scene_idx--;

    load_scene(_selected_scene_idx);
}

auto ScenePicker::next_scene() -> void
{
    if (_scene_count == 0)
        return;

    if (_selected_scene_idx >= _scene_count - 1)
        _selected_scene_idx = 0;
    else
        _selected_scene_idx++;

    load_scene(_selected_scene_idx);
}

auto ScenePicker::load_scene(usize idx) const -> void
{
    SceneManager::queue_scene(_scene_factories[idx]);
}

} // namespace zth::debug
