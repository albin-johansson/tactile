// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_texture.hpp"

#include <utility>  // move

#include <gtest/gtest.h>

#include "tactile/base/container/maybe.hpp"
#include "tactile/core/platform/sdl_context.hpp"
#include "tactile/opengl/opengl_renderer.hpp"

namespace tactile {

class OpenGLTextureTest : public testing::Test {
 protected:
  void SetUp() override
  {
    mSDL.emplace();
    if (auto renderer = OpenGLRenderer::make()) {
      mRenderer.emplace(std::move(*renderer));
    }
    else {
      FAIL() << "Could not initialize OpenGL renderer";
    }
  }

  Maybe<SDLContext> mSDL {};
  Maybe<OpenGLRenderer> mRenderer {};
};

/** \trace tactile::OpenGLTexture::load */
TEST_F(OpenGLTextureTest, Load)
{
  const auto texture = OpenGLTexture::load("assets/images/dummy.png");
  ASSERT_TRUE(texture.has_value());
  EXPECT_NE(texture->get_handle(), nullptr);
  EXPECT_EQ(texture->get_path(), "assets/images/dummy.png");
  EXPECT_EQ(texture->get_size().width, 96);
  EXPECT_EQ(texture->get_size().height, 64);
}

}  // namespace tactile
