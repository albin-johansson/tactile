// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl_renderer/opengl_error.hpp"

#include <utility>  // to_underlying

#include <glad/glad.h>
#include <gtest/gtest.h>

namespace tactile {

/** \trace tactile::map_opengl_error_code */
TEST(OpenGLError, MapOpenGLErrorCode)
{
  EXPECT_EQ(map_opengl_error_code(GL_INVALID_ENUM), OpenGLError::kInvalidEnum);
  EXPECT_EQ(map_opengl_error_code(GL_INVALID_VALUE), OpenGLError::kInvalidValue);
  EXPECT_EQ(map_opengl_error_code(GL_INVALID_OPERATION), OpenGLError::kInvalidOperation);
  EXPECT_EQ(map_opengl_error_code(GL_STACK_OVERFLOW), OpenGLError::kStackOverflow);
  EXPECT_EQ(map_opengl_error_code(GL_STACK_UNDERFLOW), OpenGLError::kStackUnderflow);
  EXPECT_EQ(map_opengl_error_code(GL_OUT_OF_MEMORY), OpenGLError::kOutOfMemory);
  EXPECT_EQ(map_opengl_error_code(GL_INVALID_FRAMEBUFFER_OPERATION),
            OpenGLError::kInvalidFramebufferOperation);
}

/** \trace tactile::make_error [OpenGLError] */
TEST(OpenGLError, MakeError)
{
  const auto error = make_error(OpenGLError::kNotReady);
  EXPECT_EQ(error.value(), std::to_underlying(OpenGLError::kNotReady));
  EXPECT_STREQ(error.category().name(), "opengl");
}

}  // namespace tactile
