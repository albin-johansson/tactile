#include <GL/glew.h>  // glewInit

#include <centurion.hpp>  // library, window, gl_context
#include <chrono>         // zoned_time, system_clock, current_zone
#include <format>         // format
#include <iostream>       // clog
#include <string_view>    // string_view
#include <utility>        // move

#include "application.hpp"
#include "gui/imgui_wrapper.hpp"
#include "init_open_gl_attributes.hpp"

namespace {

[[nodiscard]] auto to_string(const SDL_LogPriority priority) noexcept
    -> std::string_view
{
  switch (priority)
  {
    default:
      [[fallthrough]];

    case SDL_LOG_PRIORITY_INFO:
      return "INFO";

    case SDL_LOG_PRIORITY_VERBOSE:
      return "VERBOSE";

    case SDL_LOG_PRIORITY_DEBUG:
      return "DEBUG";

    case SDL_LOG_PRIORITY_WARN:
      return "WARNING";

    case SDL_LOG_PRIORITY_ERROR:
      return "ERROR";

    case SDL_LOG_PRIORITY_CRITICAL:
      return "CRITICAL";
  }
}

void tactile_log(void* /*data*/,
                 const int /*category*/,
                 const SDL_LogPriority priority,
                 const char* msg)
{
  using std::chrono::current_zone;
  using std::chrono::system_clock;
  using std::chrono::zoned_time;

  const zoned_time zoned{current_zone(), system_clock::now()};

  std::clog << std::format("LOG {:%T} [{}] > {}\n",
                           zoned,
                           to_string(priority),
                           msg);
}

}  // namespace

auto main(int argc, char** argv) -> int
{
  cen::library centurion;

  SDL_LogSetOutputFunction(tactile_log, nullptr);
  tactile::InitOpenGLAttributes();

  constexpr auto flags = cen::window::default_flags() | cen::window::opengl |
                         cen::window::resizable | cen::window::high_dpi;
  cen::window window{"Tactile", cen::window::default_size(), flags};

  cen::gl_context context{window};
  context.make_current(window);

  if (glewInit() != GLEW_OK)
  {
    cen::log::error("Failed to initialize GLEW!");
    return -1;
  }

  tactile::ImGuiWrapper wrapper{window, context};
  window.maximize();

  tactile::Application application{std::move(window), std::move(context)};
  return application.Run();
}