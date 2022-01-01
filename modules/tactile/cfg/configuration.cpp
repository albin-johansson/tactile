#include "configuration.hpp"

#include <cassert>    // assert
#include <cstdlib>    // abort
#include <exception>  // set_terminate

#include <GL/glew.h>
#include <tactile_stdlib.hpp>

#include "attributes.hpp"
#include "build.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"

namespace Tactile {
namespace {

[[nodiscard]] consteval auto GetWindowFlags() noexcept
{
  auto flags = cen::Window::Hidden | cen::Window::Resizable | cen::Window::OpenGL;

  if constexpr (IsPlatformOSX()) {
    flags |= cen::Window ::AllowHighDPI;
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

  mWindow.emplace("Tactile", cen::Window::GetDefaultSize(), GetWindowFlags());
  mWindow->SetIcon(cen::Surface{"resources/icon.png"});

  assert(mWindow.has_value());
  mOpenGL.emplace(*mWindow);
  mOpenGL->MakeCurrent(*mWindow);

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
