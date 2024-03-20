// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_renderer.hpp"

#include <gtest/gtest.h>

#include "tactile/base/container/maybe.hpp"
#include "tactile/core/platform/sdl_context.hpp"

namespace tactile {

class OpenGLRendererTest : public testing::Test {
 protected:
  void SetUp() override { mSDL.emplace(); }

  Maybe<SDLContext> mSDL {};
};

/** \trace tactile::OpenGLRenderer::load_texture */
TEST_F(OpenGLRendererTest, LoadTexture)
{
  auto renderer = OpenGLRenderer::make();
  ASSERT_TRUE(renderer.has_value());
  EXPECT_NE(renderer->load_texture("assets/images/dummy.png"), nullptr);
  EXPECT_EQ(renderer->load_texture("a/b/c.png"), nullptr);
}

}  // namespace tactile
