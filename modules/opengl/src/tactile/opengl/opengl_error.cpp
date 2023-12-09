// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_error.hpp"

#include <glad/glad.h>

namespace tactile {
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
      case OpenGLError::kUnknown: return "unknown";
      case OpenGLError::kInvalidEnum: return "invalid enum";
      case OpenGLError::kInvalidValue: return "invalid value";
      case OpenGLError::kInvalidOperation: return "invalid operation";
      case OpenGLError::kStackOverflow: return "stack overflow";
      case OpenGLError::kStackUnderflow: return "stack underflow";
      case OpenGLError::kOutOfMemory: return "out of memory";
      case OpenGLError::kInvalidFramebufferOperation:
        return "invalid framebuffer operation";
    }

    return "unknown";
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

}  // namespace tactile
