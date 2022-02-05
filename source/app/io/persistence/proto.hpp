#pragma once

#include "meta/build.hpp"

#if TACTILE_COMPILER_MSVC

#pragma warning(push)
#pragma warning(disable : 4127)  // conditional expression is constant
#pragma warning(disable : 5054)  // operator '*' between enumerations of different types

#endif  // TACTILE_COMPILER_MSVC

#include <history.pb.h>
#include <session.pb.h>
#include <settings.pb.h>

#if TACTILE_COMPILER_MSVC

#pragma warning(pop)

#endif  // TACTILE_COMPILER_MSVC
