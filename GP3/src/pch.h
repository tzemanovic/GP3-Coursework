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
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

typedef HWND			WindowHandle;
typedef HDC				DeviceContextHandle;
typedef HGLRC			OpenGlContextHandle; 
typedef const wchar_t*	String;
typedef unsigned		GameObjectId;

// for quick debugging
#include <iostream>

#include <cstdint>
#include <string>

#include <memory>
#include <utility>

#include <tuple>
#include <array>
#include <queue>
#include <map>