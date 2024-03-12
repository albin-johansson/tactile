// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <cstdlib>    // EXIT_SUCCESS, EXIT_FAILURE
#include <exception>  // exception

#include <centurion/message_box.hpp>
#include <fmt/format.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/util/fmt.hpp"
#include "init/app_initializer.hpp"
#include "runtime/app.hpp"
#include "tactile/core/debug/exception.hpp"

namespace {

void show_crash_message_box(const char* error_msg)
{
  cen::message_box::show(
      "Tactile crashed :(",
      fmt::format("Error message: {}\nPlease consider submitting a bug "
                  "report with reproduction steps at "
                  "https://github.com/albin-johansson/tactile",
                  error_msg),
      cen::message_box_type::error);
}

}  // namespace

auto main(int, char*[]) -> int
{
  try {
    tactile::AppInitializer initializer;

    tactile::App app {initializer.get_window()};
    app.start();

    return EXIT_SUCCESS;
  }
  catch (const tactile::Exception& e) {
    show_crash_message_box(e.what());
    spdlog::critical("Unhandled exception message: '{}'\n{}", e.what(), e.trace());
    return EXIT_FAILURE;
  }
  catch (const std::exception& e) {
    show_crash_message_box(e.what());
    spdlog::critical("Unhandled exception message: '{}'", e.what());
    return EXIT_FAILURE;
  }
  catch (...) {
    show_crash_message_box("N/A");
    spdlog::critical("Unhandled value exception!");
    return EXIT_FAILURE;
  }
}
