#include <gtest/gtest.h>

#include "cfg/configuration.hpp"

auto main(int argc, char** argv) -> int
{
  tactile::ApplicationConfiguration configuration;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}