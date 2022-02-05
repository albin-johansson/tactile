#pragma once

#include "meta/build.hpp"

#if TACTILE_COMPILER_GCC || TACTILE_COMPILER_CLANG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif  // TACTILE_COMPILER_GCC || TACTILE_COMPILER_CLANG

#include <boost/assert.hpp>
#include <boost/exception/all.hpp>
#include <boost/stacktrace.hpp>

#if TACTILE_COMPILER_GCC || TACTILE_COMPILER_CLANG
#pragma GCC diagnostic pop
#endif  // TACTILE_COMPILER_GCC || TACTILE_COMPILER_CLANG
