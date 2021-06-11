#include "tactile_app.hpp"

auto main(int argc, char** argv) -> int
{
  tactile::TactileApp app{argc, argv};
  return tactile::TactileApp::exec();
}