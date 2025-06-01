#include "zenith/core/profiler.hpp"

#if defined(ZTH_PROFILER)

#include <imgui.h>

#include "zenith/core/assert.hpp"
#include "zenith/debug/ui.hpp"
#include "zenith/system/application.hpp"

namespace zth {

Vector<EntryMarkerIndex> Profiler::_opening_marker_index_stack;
Vector<ProfilerEntry> Profiler::_entry_stack;

Vector<ProfilerEntryMarker> Profiler::_this_frame_markers;
Vector<ProfilerEntryMarker> Profiler::_last_frame_markers;

auto ProfilerEntryMarker::make_opening_marker(const char* label) -> ProfilerEntryMarker
{
    return ProfilerEntryMarker{
        .label = label,
    };
}

auto ProfilerEntryMarker::make_closing_marker(double time) -> ProfilerEntryMarker
{
    return ProfilerEntryMarker{
        .closing_marker_index = index_value_for_closing_marker,
        .time = time,
    };
}

auto ProfilerEntryMarker::is_an_opening_marker() const -> bool
{
    return closing_marker_index != index_value_for_closing_marker;
}

auto ProfilerEntryMarker::is_a_closing_marker() const -> bool
{
    return closing_marker_index == index_value_for_closing_marker;
}

ScopeProfiler::ScopeProfiler(const char* scope_name)
{
    Profiler::begin_entry(scope_name);
    _start_time = Application::time();
}

ScopeProfiler::~ScopeProfiler()
{
    auto duration = Application::time() - _start_time;
    Profiler::end_entry(duration);
}

auto Profiler::init() -> void
{
    begin_profile();
}

auto Profiler::start_frame() -> void
{
    end_profile();

    ZTH_ASSERT(_opening_marker_index_stack.empty());
    ZTH_ASSERT(_entry_stack.empty());

    begin_profile();
}

auto Profiler::display() -> void
{
    ZTH_PROFILE_FUNCTION();

    debug::begin_window("Profiler");

    debug::text("Frame time: {:.4f}ms", Application::frame_time() * 1000.0);

    ImGui::SameLine(ImGui::GetWindowWidth() - 100.0f);

    if (debug::button("Snapshot"))
        _snapshot = !_snapshot;

    merge_and_display_sub_entries({ 0 });

    debug::end_window();
}

auto Profiler::begin_entry(const char* label) -> void
{
    auto marker_index = static_cast<EntryMarkerIndex>(_this_frame_markers.size());
    _this_frame_markers.push_back(ProfilerEntryMarker::make_opening_marker(label));
    _opening_marker_index_stack.push_back(marker_index);
}

auto Profiler::end_entry(double time) -> void
{
    auto opening_marker_index = _opening_marker_index_stack.back();
    _opening_marker_index_stack.pop_back();
    auto marker_index = _this_frame_markers.size();

    // Write back the index of this marker to the corresponding opening marker.
    _this_frame_markers[opening_marker_index].closing_marker_index = static_cast<EntryMarkerIndex>(marker_index);
    _this_frame_markers.push_back(ProfilerEntryMarker::make_closing_marker(time));
}

auto Profiler::begin_profile() -> void
{
    if (!_snapshot)
        swap(_last_frame_markers, _this_frame_markers);

    _this_frame_markers.clear();
    begin_entry("Root node");
}

auto Profiler::end_profile() -> void
{
    end_entry(0.0);
}

auto Profiler::merge_and_display_sub_entries(const TemporaryVector<EntryMarkerIndex>& indices) -> void
{
    // Display together sub entries of the entries pointed to by indices.

#if defined(ZTH_ASSERTIONS)
    // The indices should always point to opening markers.
    for (auto index : indices)
    {
        ZTH_ASSERT(index < _last_frame_markers.size());
        ZTH_ASSERT(_last_frame_markers[index].is_an_opening_marker());
    }
#endif

    const auto entry_stack_begin_index = _entry_stack.size();

    // Collect the entries.
    for (auto index : indices)
    {
        auto parent_closing_marker_index = _last_frame_markers[index].closing_marker_index;

        for (index++; index < parent_closing_marker_index; index++)
        {
            const auto& opening_marker = _last_frame_markers[index];
            ZTH_ASSERT(opening_marker.is_an_opening_marker());

            auto closing_marker_index = opening_marker.closing_marker_index;
            auto& closing_marker = _last_frame_markers[closing_marker_index];
            ZTH_ASSERT(closing_marker.is_a_closing_marker());

            // Find the entry on the stack with the same label and merge them, otherwise make a new entry.
            auto merge_target = std::ranges::find(_entry_stack.begin() + static_cast<isize>(entry_stack_begin_index),
                                                  _entry_stack.end(), opening_marker.label, &ProfilerEntry::label);

            if (merge_target != _entry_stack.end())
            {
                merge_target->time += closing_marker.time;
                merge_target->source_opening_marker_indices.push_back(index);
            }
            else
            {
                _entry_stack.push_back(ProfilerEntry{
                    .label = opening_marker.label,
                    .time = closing_marker.time,
                    .source_opening_marker_indices = { index },
                });
            }

            // Skip the sub entries.
            index = closing_marker_index;
        }
    }

    // Sort the entries by time.
    std::ranges::sort(_entry_stack.begin() + static_cast<isize>(entry_stack_begin_index), _entry_stack.end(),
                      [](const auto& a, const auto& b) { return a.time > b.time; });

    // Display the entries.
    for (auto index = entry_stack_begin_index; index < _entry_stack.size(); index++)
    {
        const auto& current_entry = _entry_stack[index];

        auto entry_label = format_to_temporary("{}", current_entry.label);
        auto node_opened = ImGui::TreeNodeEx(entry_label.c_str(), ImGuiTreeNodeFlags_DefaultOpen);

        ImGui::SameLine(ImGui::GetWindowWidth() - 100.0f);
        debug::text("{:.4f}ms", current_entry.time * 1000.0);

        if (node_opened)
        {
            merge_and_display_sub_entries(current_entry.source_opening_marker_indices);
            ImGui::TreePop();
        }
    }

    // Clear the entry stack for this frame.
    _entry_stack.resize(entry_stack_begin_index);
}

} // namespace zth

#endif
