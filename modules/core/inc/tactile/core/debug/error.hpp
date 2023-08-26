// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>  // exception

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/string.hpp"

namespace tactile {

class TACTILE_CORE_API Error final : public std::exception {
 public:
  TACTILE_NOINLINE explicit Error(String message);

  [[nodiscard]] auto what() const noexcept -> const char* override;

  [[nodiscard]] auto get_trace() const -> const String&;

 private:
  String mMessage;
  String mTrace;
};

}  // namespace tactile