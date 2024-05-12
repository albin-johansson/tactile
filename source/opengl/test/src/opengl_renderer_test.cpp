// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_renderer.hpp"

#include <SDL2/SDL.h>
#include <gtest/gtest.h>
#include <imgui.h>

#include "tactile/base/container/maybe.hpp"
#include "tactile/core/platform/sdl_context.hpp"
#include "tactile/core/platform/window.hpp"
#include "tactile/core/ui/imgui_context.hpp"

namespace tactile {

class OpenGLRendererTest : public testing::Test
{
 protected:
  void SetUp() override
  {
    mSDL.emplace();

    if (auto window = Window::create(SDL_WINDOW_OPENGL)) {
      mWindow.emplace(std::move(*window));
    }
    else {
      FAIL() << "Could not initialize OpenGL renderer";
    }

    mImGuiContext.reset(ImGui::CreateContext());
  }

  Maybe<SDLContext> mSDL {};
  Maybe<Window> mWindow {};
  ui::UniqueImGuiContext mImGuiContext {};
};

/// \trace tactile::OpenGLRenderer::load_texture
TEST_F(OpenGLRendererTest, LoadTexture)
{
  auto renderer = OpenGLRenderer::make(&mWindow.value(), mImGuiContext.get());
  ASSERT_TRUE(renderer.has_value());

  EXPECT_NE(renderer->load_texture("assets/images/dummy.png"), nullptr);
  EXPECT_EQ(renderer->load_texture("a/b/c.png"), nullptr);
}

}  // namespace tactile
