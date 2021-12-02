#include "application.hpp"
#include "cfg/configuration.hpp"

auto main(int, char**) -> int
{
  Tactile::ApplicationConfiguration configuration;
  Tactile::Application app{&configuration};
  return app.Run();
}