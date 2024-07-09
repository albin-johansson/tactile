// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <SDL2/SDL_main.h>

#include "tactile/runtime/launcher.hpp"

auto main(const int argc, char* argv[]) -> int
{
  return tactile::launch(argc, argv);
}
