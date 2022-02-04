#pragma once

#include <exception>  // exception

#include "boost.hpp"
#include "build.hpp"

namespace tactile {

class tactile_error : public std::exception {
 public:
  tactile_error() noexcept = default;

  explicit tactile_error(const char* what) : mWhat{what ? what : "N/A"} {}

  [[nodiscard]] auto what() const noexcept -> const char* override { return mWhat; }

 private:
  const char* mWhat{"N/A"};
};

namespace tags {
struct trace_info_tag;
}  // namespace tags

using trace_info = boost::error_info<tags::trace_info_tag, boost::stacktrace::stacktrace>;

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
