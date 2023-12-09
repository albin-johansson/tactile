// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>  // exception

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class TACTILE_FOUNDATION_API Exception final : public std::exception {
 public:
  explicit Exception(String message);

  [[nodiscard]]
  auto what() const noexcept -> const char* override;

  [[nodiscard]]
  auto get_trace() const -> const String&;

 private:
  String mMessage;
  String mTrace;
};

}  // namespace tactile
