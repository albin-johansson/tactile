#pragma once

#include "build.hpp"

#if TACTILE_COMPILER_GCC || TACTILE_COMPILER_CLANG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif  // TACTILE_COMPILER_GCC || TACTILE_COMPILER_CLANG

#include <boost/exception/all.hpp>
#include <boost/stacktrace.hpp>

#if TACTILE_COMPILER_GCC || TACTILE_COMPILER_CLANG
#pragma GCC diagnostic pop
#endif  // TACTILE_COMPILER_GCC || TACTILE_COMPILER_CLANG

namespace Tactile {

using TraceInfo = boost::error_info<struct ErrorInfoTag, boost::stacktrace::stacktrace>;

template <typename Exception>
[[noreturn]] void ThrowTraced(const Exception& e)
{
  throw boost::enable_error_info(e) << TraceInfo{boost::stacktrace::stacktrace()};
}

}  // namespace Tactile
