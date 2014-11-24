///
/// precompiled header
///
#ifndef PCH_H
#define PCH_H

// Windows
#define PW_PLATFORM_WIN32
// exclude rarely-used services from Windows headers
#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <cstdlib>

// for quick debugging
#include <iostream>

#include <cstdint>
#include <string>

#include <utility>

#include <tuple>
#include <array>
#include <queue>

#endif