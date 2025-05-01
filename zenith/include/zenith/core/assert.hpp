#pragma once

#include <cstdlib>
#include <iostream>
#include <source_location>
#include <type_traits>

#include "zenith/util/macros.hpp"

// We're using <iostream> instead of <print> to print assertion messages because it's more efficient and generates less
// code.

#define ZTH_INTERNAL_ASSERT_IMPL(...)                                                                                  \
    {                                                                                                                  \
        if ((__VA_ARGS__)) [[likely]]                                                                                  \
        {}                                                                                                             \
        else [[unlikely]]                                                                                              \
        {                                                                                                              \
            std::source_location source_location = std::source_location::current();                                    \
            std::cerr << source_location.file_name() << '(' << source_location.line() << ':'                           \
                      << source_location.column() << ") `" << source_location.function_name()                          \
                      << "`:\nAssertion failed: (" << #__VA_ARGS__ << ")\n";                                           \
            ZTH_DEBUG_BREAK;                                                                                           \
            std::abort();                                                                                              \
        }                                                                                                              \
    }                                                                                                                  \
    ZTH_NOP

// ZTH_RUNTIME_ASSERT

#define ZTH_RUNTIME_ASSERT(...) ZTH_INTERNAL_ASSERT_IMPL(__VA_ARGS__)

// ZTH_ASSERT

#if defined(ZTH_ASSERTIONS)

#define ZTH_ASSERT(...) ZTH_INTERNAL_ASSERT_IMPL(__VA_ARGS__)

#else

#define ZTH_ASSERT(...) ZTH_NOP

#endif

// ZTH_CONSTEVAL_OR_REGULAR_ASSERT

// @refactor: Use if consteval once it's supported.
#define ZTH_CONSTEVAL_OR_REGULAR_ASSERT(...)                                                                           \
    {                                                                                                                  \
        if (std::is_constant_evaluated())                                                                              \
        {                                                                                                              \
            throw "Compile-time assertion failed: (" #__VA_ARGS__ ")";                                                 \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            ZTH_ASSERT(__VA_ARGS__);                                                                                   \
        }                                                                                                              \
    }                                                                                                                  \
    ZTH_NOP
