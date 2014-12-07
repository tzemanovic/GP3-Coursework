///
/// precompiled header
///
#pragma once

// exclude rarely-used services from Windows headers
#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glu.h>

typedef HWND WindowHandle;
typedef HDC DeviceContextHandle;
typedef HGLRC OpenGlContextHandle; 
typedef const wchar_t* string;

// for quick debugging
#include <iostream>

#include <cstdint>
#include <string>

#include <utility>

#include <tuple>
#include <array>
#include <queue>