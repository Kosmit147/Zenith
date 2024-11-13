#pragma once

#define ZTH_NO_COPY(type)                                                                                              \
    type(const type&) = delete;                                                                                        \
    auto operator=(const type&) = delete;

#define ZTH_NO_MOVE(type)                                                                                              \
    type(type&&) = delete;                                                                                             \
    auto operator=(type&&) = delete;

#define ZTH_NO_COPY_NO_MOVE(type)                                                                                      \
    ZTH_NO_COPY(type);                                                                                                 \
    ZTH_NO_MOVE(type);

#define ZTH_DEFAULT_COPY(type)                                                                                         \
    type(const type&) = default;                                                                                       \
    auto operator=(const type&) = default;

#define ZTH_DEFAULT_MOVE(type)                                                                                         \
    type(type&&) = default;                                                                                            \
    auto operator=(type&&) = default;

#define ZTH_DEFAULT_COPY_DEFAULT_MOVE(type)                                                                            \
    ZTH_DEFAULT_COPY(type);                                                                                            \
    ZTH_DEFAULT_MOVE(type);
