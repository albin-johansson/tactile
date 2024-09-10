// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code, error_category
#include <utility>       // to_underlying

#include "tactile/base/prelude.hpp"
#include "tactile/opengl_renderer/api.hpp"

namespace tactile {

/**
 * Provides error codes for OpenGL errors.
 */
enum class OpenGLError : int
{
  kUnknown,
  kInvalidParam,
  kNotReady,
  kLoaderError,
  kWindowError,
  kContextError,
  kImGuiError,
  kBadImage,
  kInvalidEnum,
  kInvalidValue,
  kInvalidOperation,
  kStackOverflow,
  kStackUnderflow,
  kOutOfMemory,
  kInvalidFramebufferOperation,
};

/**
 * Returns the OpenGL error category.
 *
 * \return
 * An error category.
 */
[[nodiscard]]
TACTILE_OPENGL_API auto get_opengl_error_category() noexcept -> const std::error_category&;

/**
 * Translates an OpenGL error code, e.g. \c GL_INVALID_ENUM, to a Tactile OpenGL
 * error.
 *
 * \param error A native OpenGL error code identifier.
 *
 * \return
 * An OpenGL error code.
 */
[[nodiscard]]
TACTILE_OPENGL_API auto map_opengl_error_code(unsigned error) noexcept -> OpenGLError;

/**
 * Creates an OpenGL error code.
 *
 * \param error An OpenGL error identifier.
 *
 * \return
 * An error code.
 */
[[nodiscard]]
inline auto make_error(const OpenGLError error) noexcept -> std::error_code
{
  return std::error_code {std::to_underlying(error), get_opengl_error_category()};
}

}  // namespace tactile
