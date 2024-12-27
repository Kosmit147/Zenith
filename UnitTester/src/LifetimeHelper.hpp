#pragma once

#include <cstddef>

struct LifetimeHelper
{
    LifetimeHelper()
    {
        default_ctors++;
        ctors++;
    }

    LifetimeHelper(const LifetimeHelper&)
    {
        copy_ctors++;
        ctors++;
    }

    auto operator=(const LifetimeHelper&) -> LifetimeHelper&
    {
        copy_assignments++;
        return *this;
    }

    LifetimeHelper(LifetimeHelper&&) noexcept
    {
        move_ctors++;
        ctors++;
    }

    auto operator=(LifetimeHelper&&) noexcept -> LifetimeHelper&
    {
        move_assignments++;
        return *this;
    }

    ~LifetimeHelper() { dtors++; }

    static auto reset() -> void
    {
        ctors = 0;

        default_ctors = 0;
        copy_ctors = 0;
        move_ctors = 0;

        copy_assignments = 0;
        move_assignments = 0;

        dtors = 0;
    }

    static inline std::size_t ctors = 0;

    static inline std::size_t default_ctors = 0;
    static inline std::size_t copy_ctors = 0;
    static inline std::size_t move_ctors = 0;

    static inline std::size_t copy_assignments = 0;
    static inline std::size_t move_assignments = 0;

    static inline std::size_t dtors = 0;
};
