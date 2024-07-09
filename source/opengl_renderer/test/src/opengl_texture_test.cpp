// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl_renderer/opengl_texture.hpp"

#include <gtest/gtest.h>

#include "tactile/opengl_renderer/opengl_renderer_plugin.hpp"
#include "tactile/runtime/runtime.hpp"

namespace tactile {

class OpenGLTextureTest : public testing::Test
{
 protected:
  void SetUp() override
  {
    mPlugin.load(mRuntime);
  }

  void TearDown() override
  {
    mPlugin.unload(mRuntime);
  }

  Runtime mRuntime {};
  OpenGLRendererPlugin mPlugin {};
};

/// \trace tactile::OpenGLTexture::load
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
