#pragma once

#if defined(ZTH_PROFILER)

#include "zenith/core/typedefs.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/system/temporary_storage.hpp"
#include "zenith/util/macros.hpp"

// The profiler is designed in such a way as to be the least invasive and have the least amount of impact on the
// performance of the measured functions/scopes as well as on the performance of the whole application.
//
// The profiler stores two arrays of entry markers: one for last frame's entries and one for this frame's entries. At
// the beginning of every frame these arrays are swapped and the array containing this frame's markers is cleared. These
// entry markers delineate profiler entries. There are two types of markers: an opening marker and a closing marker. An
// opening marker contains the label of the measured scope and an index containing the position of the corresponding
// closing marker in the marker array. This is needed in order to be able to skip the sub entries of the nodes which
// aren't open when iterating over the entries. A closing marker contains the measured time. Whenever a new measurement
// begins an opening marker is added to the array and when the measurement is finished a closing marker is added to the
// array. Other pairs of opening and closing markers can be added in the meantime. This way the markers create a tree
// structure of profiler entries. When the time comes to display the entries, the profiler constructs this tree
// structure out of the marker array.

namespace zth {

using EntryMarkerIndex = u32;

struct ProfilerEntryMarker
{
    EntryMarkerIndex closing_marker_index; // Index of the corresponding closing marker or -1 if it's a closing marker.

    union
    {
        const char* label; // If it's an opening marker.
        double time;       // If it's a closing marker, in seconds.
    };

    [[nodiscard]] static auto make_opening_marker(const char* label) -> ProfilerEntryMarker;
    [[nodiscard]] static auto make_closing_marker(double time) -> ProfilerEntryMarker;

    [[nodiscard]] auto is_an_opening_marker() const -> bool;
    [[nodiscard]] auto is_a_closing_marker() const -> bool;

private:
    static constexpr auto index_value_for_closing_marker = ~static_cast<EntryMarkerIndex>(0);
};

struct ProfilerEntry
{
    EntryMarkerIndex opening_marker_index; // Index of the corresponding opening marker in the marker array.
    const char* label;
    double time; // In seconds.
};

struct MergedProfilerEntry
{
    const char* label;
    double time; // In seconds.
    TemporaryVector<EntryMarkerIndex> opening_marker_indices;
};

class ScopeProfiler
{
public:
    explicit ScopeProfiler(const char* scope_name);
    ZTH_NO_COPY_NO_MOVE(ScopeProfiler)
    ~ScopeProfiler();

private:
    double _start_time;
};

class Profiler
{
public:
    Profiler() = delete;

    static auto init() -> void;
    static auto start_frame() -> void;
    static auto display() -> void;

    static auto begin_entry(const char* label) -> void;
    static auto end_entry(double time) -> void;

private:
    // This is used to get to the corresponding opening marker whenever we end an entry.
    static Vector<EntryMarkerIndex> _opening_marker_index_stack;

    static Vector<ProfilerEntry> _entry_stack;
    static Vector<MergedProfilerEntry> _merged_entry_stack;

    static Vector<ProfilerEntryMarker> _this_frame_markers;
    static Vector<ProfilerEntryMarker> _last_frame_markers;

private:
    static auto begin_profile() -> void;
    static auto end_profile() -> void;

    static auto merge_and_display_sub_entries(const TemporaryVector<EntryMarkerIndex>& parent_indices) -> void;
};

} // namespace zth

// clang-format off

#define ZTH_PROFILE_SCOPE(scope_name) zth::ScopeProfiler ZTH_UNIQUE_NAME(scope_profiler_){ scope_name }
#define ZTH_PROFILE_FUNCTION() zth::ScopeProfiler ZTH_UNIQUE_NAME(scope_profiler_){ __FUNCTION__ }

// clang-format on

#else

#define ZTH_PROFILE_SCOPE()
#define ZTH_PROFILE_FUNCTION()

#endif
