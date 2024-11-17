#pragma once

#include <stacktrace>
#include <stdexcept>
#include <string>

#include "Zenith/Core/Typedefs.hpp"

namespace zth {

class Exception : public std::runtime_error
{
public:
    explicit Exception(const char* message, usize skip = 0);
    explicit Exception(const std::string& message, usize skip = 0);

    [[nodiscard]] auto stacktrace() const -> std::string;

private:
    std::stacktrace _stacktrace;
};

} // namespace zth
