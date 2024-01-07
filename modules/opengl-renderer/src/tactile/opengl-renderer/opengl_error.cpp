// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl-renderer/opengl_error.hpp"

#include <glad/glad.h>

namespace tactile::opengl {
namespace {

class OpenGLErrorCategory final : public std::error_category {
 public:
  [[nodiscard]]
  auto name() const noexcept -> const char* override
  {
    return "opengl";
  }

  [[nodiscard]]
  auto message(const int value) const -> std::string override
  {
    switch (OpenGLError {value}) {
      using enum OpenGLError;
      case kUnknown: return "unknown";
      case kInvalidEnum: return "invalid enum";
      case kInvalidValue: return "invalid value";
      case kInvalidOperation: return "invalid operation";
      case kStackOverflow: return "stack overflow";
      case kStackUnderflow: return "stack underflow";
      case kOutOfMemory: return "out of memory";
      case kInvalidFramebufferOperation: return "invalid framebuffer operation";
    }

    return "?";
  }
};

}  // namespace

auto to_opengl_error(const uint value) noexcept -> OpenGLError
{
  switch (value) {
    case GL_INVALID_ENUM: return OpenGLError::kInvalidEnum;
    case GL_INVALID_VALUE: return OpenGLError::kInvalidValue;
    case GL_INVALID_OPERATION: return OpenGLError::kInvalidOperation;
    case GL_STACK_OVERFLOW: return OpenGLError::kStackOverflow;
    case GL_STACK_UNDERFLOW: return OpenGLError::kStackUnderflow;
    case GL_OUT_OF_MEMORY: return OpenGLError::kOutOfMemory;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      return OpenGLError::kInvalidFramebufferOperation;
  }

  return OpenGLError::kUnknown;
}

auto get_opengl_error_category() noexcept -> const std::error_category&
{
  static OpenGLErrorCategory opengl_error_category {};
  return opengl_error_category;
}

}  // namespace tactile::opengl
