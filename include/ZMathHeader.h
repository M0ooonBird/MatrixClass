#pragma once

#ifdef _WIN32
#ifdef ZMATH_EXPORTS
#define ZMATH_API __declspec(dllexport)
#else
#define ZMATH_API __declspec(dllimport)
#endif
#else
#define ZMATH_API
#endif

typedef double T;