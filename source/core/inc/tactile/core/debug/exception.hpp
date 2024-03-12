// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>  // exception

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/** The exception type used for all explicitly thrown exceptions in the codebase. */
class Exception final : public std::exception {
 public:
  TACTILE_NOINLINE explicit Exception(String message);

  [[nodiscard]]
  auto what() const noexcept -> const char* override;

  [[nodiscard]]
  auto trace() const -> const String&;

 private:
  String mMessage;
  String mTrace;
};

}  // namespace tactile
