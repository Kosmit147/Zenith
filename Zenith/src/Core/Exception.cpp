#include "Zenith/Core/Exception.hpp"

namespace zth {

Exception::Exception(const char* message, usize skip)
    : std::runtime_error(message), _stacktrace(std::stacktrace::current(skip + 1))
{}

Exception::Exception(std::string_view message, usize skip)
    : std::runtime_error(message.data()), _stacktrace(std::stacktrace::current(skip + 1))
{}

Exception::Exception(const std::string& message, usize skip)
    : std::runtime_error(message), _stacktrace(std::stacktrace::current(skip + 1))
{}

auto Exception::stacktrace() const -> std::string
{
    return std::to_string(_stacktrace);
}

} // namespace zth
