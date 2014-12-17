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
//#include <GL/glu.h>
// assimp - asset import lib
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// FreeImage - image loading lib
#include <FreeImage.h>

// IO for quick debugging
#include <iostream>
#define ERR( x ) do { std::cerr << x; } while ( 0 )
#define LOG( x ) do { std::cout << x; } while ( 0 )

#include <cstdint>
#include <string>

#include <memory>
#include <utility>
#include <functional>

#include <tuple>
#include <array>
#include <queue>
#include <map>

typedef HWND			WindowHandle;
typedef HDC				DeviceContextHandle;
typedef HGLRC			OpenGlContextHandle;
typedef std::string		String;
typedef unsigned		GameObjectId;

extern const float PI;
extern const float TWO_PI;
extern const float HALF_PI;