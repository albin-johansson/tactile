#include "configuration.hpp"

#include <cstdlib>    // abort
#include <exception>  // set_terminate

#include <GL/glew.h>
#include <tactile_stdlib.hpp>

#include "assert.hpp"
#include "build.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"
#include "sdl_attributes.hpp"
#include "throw.hpp"

namespace tactile {
namespace {

[[nodiscard]] consteval auto get_window_flags() noexcept -> uint32
{
  auto flags = cen::window::hidden | cen::window::resizable | cen::window::opengl;

  if constexpr (IsPlatformOSX()) {
    flags |= cen::window::allow_high_dpi;
  }

  return flags;
}

}  // namespace

app_configuration::app_configuration()
{
  /* Use terminate handler that doesn't do anything fancy, e.g. no logging */
  std::set_terminate([] { std::abort(); });

  if constexpr (IsDebugBuild()) {
    SetLogLevel(LogLevel::Verbose);
  }
  else {
    SetLogLevel(LogLevel::Info);
  }

  init_sdl_attributes();

  mWindow.emplace("Tactile", cen::window::default_size(), get_window_flags());
  mWindow->set_icon(cen::surface{"resources/icon.png"});

  TACTILE_ASSERT(mWindow.has_value());
  mOpenGL.emplace(*mWindow);
  mOpenGL->make_current(*mWindow);

  cen::gl::set_swap_interval(cen::gl_swap_interval::synchronized);

  if (glewInit() != GLEW_OK) {
    LogError("Failed to initialize GLEW!");
    ThrowTraced(TactileError{"Failed to initialize GLEW!"});
  }

  LogDebug("OpenGL version... {}", glGetString(GL_VERSION));
  LogDebug("OpenGL renderer... {}", glGetString(GL_RENDERER));

  LoadPreferences();

  TACTILE_ASSERT(mOpenGL.has_value());
  mImGui.emplace(*mWindow, *mOpenGL);

  mWindow->maximize();
}

auto app_configuration::window() -> cen::window&
{
  return mWindow.value();
}

}  // namespace tactile
