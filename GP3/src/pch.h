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
// OpenGL Extension Wrangler
#include <GL/glew.h>
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
// OpenGL
#include <GL/gl.h>
#include <GL/glu.h>
// Assimp
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

typedef HWND			WindowHandle;
typedef HDC				DeviceContextHandle;
typedef HGLRC			OpenGlContextHandle;
typedef LPCWSTR			String;
typedef unsigned		GameObjectId;

// IO for quick debugging
#include <iostream>
#define DEBUG( x ) do { std::cerr << x; } while ( 0 )
#define LOG( x ) do { std::cout << x; } while ( 0 )

#include <cstdint>
#include <string>

#include <memory>
#include <utility>

#include <tuple>
#include <array>
#include <queue>
#include <map>