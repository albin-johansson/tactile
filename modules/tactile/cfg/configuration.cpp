#include "configuration.hpp"

#include <cstdlib>    // abort
#include <exception>  // set_terminate

#include <GL/glew.h>
#include <tactile_stdlib.hpp>

#include "assert.hpp"
#include "attributes.hpp"
#include "build.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"
#include "throw.hpp"

namespace tactile {
namespace {

[[nodiscard]] consteval auto GetWindowFlags() noexcept
{
  auto flags = cen::window::hidden | cen::window::resizable | cen::window::opengl;

  if constexpr (IsPlatformOSX()) {
    flags |= cen::window::allow_high_dpi;
  }

  return flags;
}

}  // namespace

ApplicationConfiguration::ApplicationConfiguration()
{
  /* Use terminate handler that doesn't do anything fancy, e.g. no logging */
  std::set_terminate([] { std::abort(); });

  if constexpr (IsDebugBuild()) {
    SetLogLevel(LogLevel::Verbose);
  }
  else {
    SetLogLevel(LogLevel::Info);
  }

  InitAttributes();

  mWindow.emplace("Tactile", cen::window::default_size(), GetWindowFlags());
  mWindow->set_icon(cen::surface{"resources/icon.png"});

  TACTILE_ASSERT(mWindow.has_value());
  mOpenGL.emplace(*mWindow);
  mOpenGL->MakeCurrent(*mWindow);

  SDL_GL_SetSwapInterval(1); /* Enables VSync */

  if (glewInit() != GLEW_OK) {
    LogError("Failed to initialize GLEW!");
    ThrowTraced(TactileError{"Failed to initialize GLEW!"});
  }

  LogDebug("OpenGL version... {}", glGetString(GL_VERSION));
  LogDebug("OpenGL renderer... {}", glGetString(GL_RENDERER));

  LoadPreferences();

  TACTILE_ASSERT(mOpenGL.has_value());
  mImGui.emplace(*mWindow, *mOpenGL);

  SDL_MaximizeWindow(mWindow->get());
}

auto ApplicationConfiguration::GetWindow() -> SDL_Window*
{
  return mWindow->get();
}

}  // namespace tactile
