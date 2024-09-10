// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl_renderer/opengl_error.hpp"

#include <glad/glad.h>

namespace tactile {
namespace {

class OpenGLErrorCategory final : public std::error_category
{
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
      case OpenGLError::kUnknown:                     return "unknown";
      case OpenGLError::kInvalidParam:                return "invalid parameter";
      case OpenGLError::kNotReady:                    return "not ready";
      case OpenGLError::kLoaderError:                 return "could not load functions";
      case OpenGLError::kWindowError:                 return "could not create window";
      case OpenGLError::kContextError:                return "context error";
      case OpenGLError::kImGuiError:                  return "imgui error";
      case OpenGLError::kBadImage:                    return "bad image";
      case OpenGLError::kInvalidEnum:                 return "invalid enum";
      case OpenGLError::kInvalidValue:                return "invalid value";
      case OpenGLError::kInvalidOperation:            return "invalid operation";
      case OpenGLError::kStackOverflow:               return "stack overflow";
      case OpenGLError::kStackUnderflow:              return "stack underflow";
      case OpenGLError::kOutOfMemory:                 return "out of memory";
      case OpenGLError::kInvalidFramebufferOperation: return "invalid framebuffer operation";
    }

    return "?";
  }
};

}  // namespace

auto get_opengl_error_category() noexcept -> const std::error_category&
{
  static const OpenGLErrorCategory category {};
  return category;
}

auto map_opengl_error_code(const unsigned error) noexcept -> OpenGLError
{
  switch (error) {
    case GL_INVALID_ENUM:                  return OpenGLError::kInvalidEnum;
    case GL_INVALID_VALUE:                 return OpenGLError::kInvalidValue;
    case GL_INVALID_OPERATION:             return OpenGLError::kInvalidOperation;
    case GL_STACK_OVERFLOW:                return OpenGLError::kStackOverflow;
    case GL_STACK_UNDERFLOW:               return OpenGLError::kStackUnderflow;
    case GL_OUT_OF_MEMORY:                 return OpenGLError::kOutOfMemory;
    case GL_INVALID_FRAMEBUFFER_OPERATION: return OpenGLError::kInvalidFramebufferOperation;
  }

  return OpenGLError::kUnknown;
}

}  // namespace tactile
