// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <system_error>  // error_category, error_code
#include <utility>       // to_underlying

#include "tactile/foundation/prelude.hpp"
#include "tactile/opengl/api.hpp"

namespace tactile {

enum class OpenGLError : int {  // NOLINT(performance-enum-size)
  kUnknown,
  kInvalidEnum,
  kInvalidValue,
  kInvalidOperation,
  kStackOverflow,
  kStackUnderflow,
  kOutOfMemory,
  kInvalidFramebufferOperation,
};

[[nodiscard]]
TACTILE_OPENGL_API auto to_opengl_error(uint value) noexcept -> OpenGLError;

[[nodiscard]]
TACTILE_OPENGL_API auto get_opengl_error_category() noexcept
    -> const std::error_category&;

[[nodiscard]]
inline auto make_opengl_error(const OpenGLError error) noexcept -> std::error_code
{
  return std::error_code {std::to_underlying(error), get_opengl_error_category()};
}

}  // namespace tactile
