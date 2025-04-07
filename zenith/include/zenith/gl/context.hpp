#pragma once

#include "zenith/core/typedefs.hpp"
#include "zenith/log/format.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/util/result.hpp"

namespace zth {

namespace gl {

struct Version
{
    u32 major;
    u32 minor;

    [[nodiscard]] auto operator<=>(const Version&) const = default;
};

enum class Profile : u8
{
    Compatibility,
    Core,
};

class Context
{
public:
    Context() = delete;

    [[nodiscard]] static auto init() -> Result<void, String>;
    static auto shut_down() -> void;

    static auto set_debug_context() -> void;

    [[nodiscard]] static auto vendor_string() { return _vendor_string; }
    [[nodiscard]] static auto renderer_string() { return _renderer_string; }
    [[nodiscard]] static auto version_string() { return _version_string; }
    [[nodiscard]] static auto glsl_version_string() { return _glsl_version_string; }

    [[nodiscard]] static auto version() { return _version; }

    [[nodiscard]] static auto forward_compatible_context() { return _forward_compatible_context; }
    [[nodiscard]] static auto debug_context() { return _debug_context; }

private:
    static inline const char* _vendor_string = nullptr;
    static inline const char* _renderer_string = nullptr;
    static inline const char* _version_string = nullptr;
    static inline const char* _glsl_version_string = nullptr;

    static inline Version _version;

    static inline bool _forward_compatible_context;
    static inline bool _debug_context;

private:
    static auto retrieve_context_variables() -> void;
    static auto log_context_info() -> void; // log_context_info needs context variables to be retrieved first.
};

} // namespace gl

[[nodiscard]] auto to_string(gl::Profile gl_profile) -> const char*;

} // namespace zth

ZTH_DECLARE_FORMATTER(zth::gl::Version);
ZTH_DECLARE_FORMATTER(zth::gl::Profile);
