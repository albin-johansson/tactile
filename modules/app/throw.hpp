#pragma once

#include <exception>  // exception

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

namespace tactile {

class TactileError : public std::exception {
 public:
  TactileError() noexcept = default;

  explicit TactileError(const char* what) : mWhat{what ? what : "N/A"} {}

  [[nodiscard]] auto what() const noexcept -> const char* override { return mWhat; }

 private:
  const char* mWhat{"N/A"};
};

using TraceInfo = boost::error_info<struct ErrorInfoTag, boost::stacktrace::stacktrace>;

template <typename Exception>
[[noreturn]] void ThrowTraced(const Exception& e)
{
  throw boost::enable_error_info(e) << TraceInfo{boost::stacktrace::stacktrace()};
}

}  // namespace tactile
