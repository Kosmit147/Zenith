#include "zenith/core/exception.hpp"

#include "zenith/util/macros.hpp"

namespace zth {

ZTH_NO_INLINE Exception::Exception(const char* message, usize skip)
    : std::runtime_error(message), _stacktrace(std::stacktrace::current(skip + 1))
{}

ZTH_NO_INLINE Exception::Exception(const std::string& message, usize skip)
    : std::runtime_error(message), _stacktrace(std::stacktrace::current(skip + 1))
{}

auto Exception::stacktrace() const -> std::string
{
    return std::to_string(_stacktrace);
}

} // namespace zth
