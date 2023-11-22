// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>  // exception

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
TACTILE_NOINLINE TACTILE_CORE_API auto get_stacktrace() -> String;

class TACTILE_CORE_API Error final : public std::exception {
 public:
  explicit Error(String message);

  [[nodiscard]]
  auto what() const noexcept -> const char* override;

  [[nodiscard]]
  auto get_trace() const -> const String&;

 private:
  String mMessage;
  String mTrace;
};

using RuntimeError = Error;

}  // namespace tactile