// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>  // exception

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Captures and returns the current call stack.
 *
 * \return the call stack encoded in a string.
 */
[[nodiscard]]
TACTILE_NOINLINE TACTILE_FOUNDATION_API auto get_stacktrace() -> String;

class TACTILE_FOUNDATION_API RuntimeError final : public std::exception {
 public:
  explicit RuntimeError(String message);

  [[nodiscard]]
  auto what() const noexcept -> const char* override;

  [[nodiscard]]
  auto get_trace() const -> const String&;

 private:
  String mMessage;
  String mTrace;
};

}  // namespace tactile
