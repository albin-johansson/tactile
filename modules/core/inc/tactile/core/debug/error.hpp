// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>  // exception
#include <utility>    // move

#include "tactile/core/api.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Captures and returns the current call stack.
 *
 * \return the call stack encoded in a string.
 */
[[nodiscard]]
TACTILE_CORE_API TACTILE_NOINLINE auto get_stacktrace() -> String;

class Error final : public std::exception {
 public:
  explicit Error(String message)
    : mMessage {std::move(message)},
      mTrace {get_stacktrace()}
  {}

  [[nodiscard]]
  auto what() const noexcept -> const char* override
  {
    return mMessage.c_str();
  }

  [[nodiscard]]
  auto get_trace() const -> const String&
  {
    return mTrace;
  }

 private:
  String mMessage;
  String mTrace;
};

}  // namespace tactile