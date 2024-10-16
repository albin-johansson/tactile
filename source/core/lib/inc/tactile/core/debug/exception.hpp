// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>  // exception
#include <string>     // string

#include "tactile/base/prelude.hpp"

namespace tactile::core {

/** The exception type used for all explicitly thrown exceptions in the codebase. */
class Exception final : public std::exception
{
 public:
  TACTILE_NOINLINE explicit Exception(std::string message);

  [[nodiscard]]
  auto what() const noexcept -> const char* override;

  [[nodiscard]]
  auto trace() const -> const std::string&;

 private:
  std::string mMessage;
  std::string mTrace;
};

}  // namespace tactile::core
