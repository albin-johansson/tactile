#include <gtest/gtest.h>

#include <centurion.hpp>  // library

auto main(int argc, char** argv) -> int
{
  cen::library library;

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}