// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl_renderer/opengl_renderer.hpp"

#include <gtest/gtest.h>

#include "tactile/opengl_renderer/opengl_renderer_plugin.hpp"
#include "tactile/runtime/command_line_options.hpp"
#include "tactile/runtime/runtime.hpp"

namespace tactile {

class OpenGLRendererTest : public testing::Test
{
 protected:
  void SetUp() override
  {
    mPlugin.load(&mRuntime);
  }

  void TearDown() override
  {
    mPlugin.unload();
  }

  Runtime mRuntime {get_default_command_line_options()};
  OpenGLRendererPlugin mPlugin {};
};

/// \trace tactile::OpenGLRenderer::load_texture
TEST_F(OpenGLRendererTest, LoadTexture)
{
  auto* renderer = dynamic_cast<OpenGLRenderer*>(mRuntime.get_renderer());
  ASSERT_NE(renderer, nullptr);

  EXPECT_EQ(renderer->load_texture("assets/images/dummy.png"), TextureID {1});
  EXPECT_FALSE(renderer->load_texture("a/b/c.png").has_value());
}

}  // namespace tactile
