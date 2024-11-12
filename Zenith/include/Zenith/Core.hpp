#pragma once

#ifdef ZTH_PLATFORM_WINDOWS

#ifdef ZTH_BUILD_SO
#define ZTH_API __declspec(dllexport)
#else
#define ZTH_API __declspec(dllimport)
#endif

#else

#define ZTH_API

#endif
