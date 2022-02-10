#include "configuration.hpp"

#include <cstdlib>    // abort
#include <exception>  // set_terminate

#include <GL/glew.h>

#include "io/persistence/preferences.hpp"
#include "meta/build.hpp"
#include "misc/assert.hpp"
#include "misc/logging.hpp"
#include "misc/throw.hpp"
#include "sdl_attributes.hpp"

#if TACTILE_PLATFORM_WINDOWS

#include <SDL_syswm.h>
#include <dwmapi.h>

#endif  // TACTILE_PLATFORM_WINDOWS

namespace tactile {
namespace {

[[nodiscard]] consteval auto _get_window_flags() noexcept -> uint32
{
  auto flags = cen::window::hidden | cen::window::resizable | cen::window::opengl;

  if constexpr (on_osx) {
    flags |= cen::window::allow_high_dpi;
  }

  return flags;
}

void _win32_use_immersive_dark_mode([[maybe_unused]] cen::window& window)
{
#if TACTILE_PLATFORM_WINDOWS
  SDL_SysWMinfo wm{};
  SDL_VERSION(&wm.version);
  if (SDL_GetWindowWMInfo(window.get(), &wm)) {
    HWND hwnd = wm.info.win.window;

    cen::shared_object dwmapi{"dwmapi.dll"};

    using signature = HRESULT(HWND, DWORD, LPCVOID, DWORD);
    auto* setAttribute = dwmapi.load_function<signature>("DwmSetWindowAttribute");

    if (!setAttribute) {
      return;
    }

    BOOL mode = 1;
    setAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &mode, sizeof mode);
  }
#endif  // TACTILE_PLATFORM_WINDOWS
}

}  // namespace

app_configuration::app_configuration()
{
  /* Use terminate handler that doesn't do anything fancy, e.g. no logging */
  std::set_terminate([] { std::abort(); });

  if constexpr (is_debug_build) {
    set_log_level(log_level::verbose);
  }
  else {
    set_log_level(log_level::info);
  }

  init_sdl_attributes();

  mWindow.emplace("Tactile", cen::window::default_size(), _get_window_flags());
  TACTILE_ASSERT(mWindow.has_value());

  _win32_use_immersive_dark_mode(*mWindow);
  mWindow->set_icon(cen::surface{"resources/icon.png"});

  mOpenGL.emplace(*mWindow);
  mOpenGL->make_current(*mWindow);

  cen::gl::set_swap_interval(cen::gl_swap_interval::synchronized);

  if (glewInit() != GLEW_OK) {
    log_error("Failed to initialize GLEW!");
    throw_traced(tactile_error{"Failed to initialize GLEW!"});
  }

  log_debug("OpenGL version... {}", glGetString(GL_VERSION));
  log_debug("OpenGL renderer... {}", glGetString(GL_RENDERER));

  load_preferences();

  TACTILE_ASSERT(mOpenGL.has_value());
  mImGui.emplace(*mWindow, *mOpenGL);

  mWindow->maximize();
}

auto app_configuration::window() -> cen::window&
{
  return mWindow.value();
}

}  // namespace tactile
