#pragma once

#define ZTH_NO_COPY(type)                                                                                              \
    type(const type&) = delete;                                                                                        \
    auto operator=(const type&) = delete;

#define ZTH_NO_MOVE(type)                                                                                              \
    type(type&&) = delete;                                                                                             \
    auto operator=(type&&) = delete;

#define ZTH_NO_COPY_NO_MOVE(type)                                                                                      \
    ZTH_NO_COPY(type)                                                                                                  \
    ZTH_NO_MOVE(type)

#define ZTH_DEFAULT_COPY(type)                                                                                         \
    type(const type&) = default;                                                                                       \
    auto operator=(const type&)->type& = default;

#define ZTH_DEFAULT_MOVE(type)                                                                                         \
    type(type&&) = default;                                                                                            \
    auto operator=(type&&)->type& = default;

#define ZTH_DEFAULT_COPY_DEFAULT_MOVE(type)                                                                            \
    ZTH_DEFAULT_COPY(type)                                                                                             \
    ZTH_DEFAULT_MOVE(type)

// ZTH_DEBUG_BREAK

#if defined(_MSC_VER)

#define ZTH_DEBUG_BREAK __debugbreak()

#elif defined(__GNUC__)

#define ZTH_DEBUG_BREAK __builtin_trap()

#elif defined(__clang__)

#define ZTH_DEBUG_BREAK __builtin_debugtrap()

#else

#define ZTH_DEBUG_BREAK

#endif

#ifdef ZTH_DIST_BUILD

#undef ZTH_DEBUG_BREAK
#define ZTH_DEBUG_BREAK

#endif

// ZTH_NO_INLINE

#if defined(_MSC_VER)

#define ZTH_NO_INLINE __declspec(noinline)

#elif defined(__GNUC__) || defined(__clang__)

#define ZTH_NO_INLINE __attribute__((noinline))

#else

#define ZTH_NO_INLINE

#endif

// ZTH_UNIQUE_NAME

#define ZTH_UNIQUE_NAME_INTERNAL2(name, counter) name##counter

#define ZTH_UNIQUE_NAME_INTERNAL(name, counter) ZTH_UNIQUE_NAME_INTERNAL2(name, counter)

#define ZTH_UNIQUE_NAME(name) ZTH_UNIQUE_NAME_INTERNAL(name, __COUNTER__)
