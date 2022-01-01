#include <exception>  // exception

#include "application.hpp"
#include "cfg/configuration.hpp"
#include "logging.hpp"
#include "throw.hpp"

auto main(int, char**) -> int
{
  try {
    Tactile::ApplicationConfiguration configuration;
    Tactile::Application app{&configuration};
    return app.Run();
  }
  catch (const std::exception& e) {
    Tactile::Print(fmt::color::hot_pink, "Unhandled exception message: '{}'\n", e.what());

    if (const auto* stacktrace = boost::get_error_info<Tactile::TracedError>(e)) {
      Tactile::Print(fmt::color::hot_pink, "{}\n", *stacktrace);
    }

    throw;
  }
}