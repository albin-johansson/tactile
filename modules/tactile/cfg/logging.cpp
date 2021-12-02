#include "logging.hpp"

#include <ctime>        // time
#include <string_view>  // string_view

#include <tactile_stdlib.hpp>

#include <fmt/chrono.h>  // localtime
#include <fmt/core.h>    // print
#include <fmt/format.h>  // format

#include "build.hpp"
#include "editor/gui/widgets/log/log_dock.hpp"

namespace Tactile {
namespace {

[[nodiscard]] constexpr auto ConvertPriority(const cen::log_priority priority)
    -> std::string_view
{
  switch (priority) {
    case cen::log_priority::verbose:
      return "VERBOSE";

    case cen::log_priority::debug:
      return "DEBUG";

    case cen::log_priority::info:
      return "INFO";

    case cen::log_priority::warn:
      return "WARNING";

    case cen::log_priority::error:
      return "ERROR";

    case cen::log_priority::critical:
      return "CRITICAL";

    default:
      throw TactileError{"Did not recognize log priority!"};
  }
}

}  // namespace

void InitLogger()
{
  SDL_LogSetOutputFunction(
      [](void* /*data*/,
         int /*category*/,
         const SDL_LogPriority priority,
         const char* msg) { LogMessage(static_cast<cen::log_priority>(priority), msg); },
      nullptr);

  if constexpr (IsDebugBuild()) {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
  }
  else {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
  }
}

void LogMessage(const cen::log_priority priority, const CStr msg)
{
  auto time = std::time(nullptr);

  const auto str = fmt::format("{:%H:%M:%S} [{}]: {}\n",
                               fmt::localtime(time),
                               ConvertPriority(priority),
                               msg);
  AddLogEntry(str);

  if constexpr (IsDebugBuild()) {
    fmt::print(fmt::runtime(str));
  }
}

}  // namespace Tactile
