#include "configuration.hpp"

#include <cassert>  // assert

#include <tactile_stdlib.hpp>

#include <GL/glew.h>  // glewInit

#include "attributes.hpp"
#include "build.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"

namespace Tactile {
namespace {

[[nodiscard]] consteval auto GetWindowFlags() noexcept
{
  auto flags = cen::window::hidden | cen::window::resizable | cen::window::opengl;

  if constexpr (IsPlatformOSX()) {
    flags |= cen::window::high_dpi;
  }

  return flags;
}

}  // namespace

ApplicationConfiguration::ApplicationConfiguration()
{
  InitAttributes();

  mWindow.emplace("Tactile", cen::window::default_size(), GetWindowFlags());
  // TODO set_icon(cen::surface{"resources/icon.png"});

  assert(mWindow.has_value());
  mOpenGL.emplace(*mWindow);
  mOpenGL->make_current(*mWindow);

  SDL_GL_SetSwapInterval(1); /* Enables VSync */

  if (glewInit() != GLEW_OK) {
    LogError("Failed to initialize GLEW!");
    throw TactileError{"Failed to initialize GLEW!"};
  }

  LogDebug("OpenGL version... {}", glGetString(GL_VERSION));
  LogDebug("OpenGL renderer... {}", glGetString(GL_RENDERER));

  LoadPreferences();

  assert(mOpenGL.has_value());
  mImGui.emplace(*mWindow, *mOpenGL);

  SDL_MaximizeWindow(mWindow->get());
}

auto ApplicationConfiguration::GetWindow() -> SDL_Window*
{
  return mWindow->get();
}

}  // namespace Tactile
