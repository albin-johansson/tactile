#include <gtest/gtest.h>

#include "cfg/configuration.hpp"
#include "misc/logging.hpp"

auto main(int argc, char** argv) -> int
{
  tactile::init_logger();
  tactile::AppCfg configuration;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}