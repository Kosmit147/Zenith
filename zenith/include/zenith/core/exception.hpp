#pragma once

#include <stacktrace>
#include <stdexcept>

#include "zenith/core/typedefs.hpp"
#include "zenith/stl/string.hpp"

namespace zth {

class Exception : public std::runtime_error
{
public:
    explicit Exception(const char* message, usize skip = 0);
    explicit Exception(const String& message, usize skip = 0);

    [[nodiscard]] auto stacktrace() const -> String;

private:
    std::stacktrace _stacktrace;
};

} // namespace zth
