#include <cstdlib>    // abort
#include <exception>  // exception

#include "application.hpp"
#include "cfg/configuration.hpp"
#include "logging.hpp"
#include "throw.hpp"

auto main(int, char**) -> int
{
  try {
    tactile::ApplicationConfiguration configuration;
    tactile::Application app{&configuration};
    return app.Run();
  }
  catch (const std::exception& e) {
    tactile::Print(fmt::color::hot_pink, "Unhandled exception message: '{}'\n", e.what());

    if (const auto* stacktrace = boost::get_error_info<tactile::TraceInfo>(e)) {
      tactile::Print(fmt::color::hot_pink, "{}\n", *stacktrace);
    }

    std::abort();
  }
}