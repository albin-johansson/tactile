#include "tactile_app.hpp"

auto main(int argc, char** argv) -> int
{
  tactile::tactile_app app{argc, argv};
  return tactile::tactile_app::exec();
}