#include <cstdlib>    // abort
#include <exception>  // exception

#include "application.hpp"
#include "cfg/configuration.hpp"
#include "crash.hpp"
#include "logging.hpp"
#include "throw.hpp"

auto main(int, char**) -> int
{
  try {
    tactile::app_configuration configuration;
    tactile::application app{&configuration};
    return app.run();
  }
  catch (const std::exception& e) {
    tactile::print(fmt::color::hot_pink, "Unhandled exception message: '{}'\n", e.what());

    if (const auto* stacktrace = boost::get_error_info<tactile::trace_info>(e)) {
      tactile::print(fmt::color::hot_pink, "{}\n", *stacktrace);
      tactile::dump_crash_info(*stacktrace);
    }

    std::abort();
  }
}