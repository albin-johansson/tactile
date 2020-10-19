#include "app.hpp"

auto main(int argc, char** argv) -> int
{
  tactile::app app{argc, argv};
  return QGuiApplication::exec();
}