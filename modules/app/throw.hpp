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

class tactile_error : public std::exception {
 public:
  tactile_error() noexcept = default;

  explicit tactile_error(const char* what) : mWhat{what ? what : "N/A"} {}

  [[nodiscard]] auto what() const noexcept -> const char* override { return mWhat; }

 private:
  const char* mWhat{"N/A"};
};

using trace_info = boost::error_info<struct ErrorInfoTag, boost::stacktrace::stacktrace>;

/**
 * \brief Throws an exception with associated call stack information.
 *
 * \param error the exception to throw.
 *
 * \see trace_info
 */
[[noreturn]] void throw_traced(const auto& error)
{
  throw boost::enable_error_info(error) << trace_info{boost::stacktrace::stacktrace()};
}

}  // namespace tactile
