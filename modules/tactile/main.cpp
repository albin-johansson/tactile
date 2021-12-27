#include "application.hpp"
#include "cfg/configuration.hpp"

int main(int, char**)
{
  Tactile::ApplicationConfiguration configuration;
  Tactile::Application app{&configuration};
  return app.Run();
}