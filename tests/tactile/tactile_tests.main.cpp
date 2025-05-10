// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

import std;
import tactile;

namespace tactile::tests {
namespace {

[[nodiscard]]
auto run(int argc, char* argv[]) -> int
{
  auto& logger = get_logger();
  logger.set_start_time(Logger::clock_type::now());
  logger.set_min_level(LogLevel::kTrace);
  logger.set_flush_level(LogLevel::kError);
  logger.add_sink(make_unique<ConsoleLogSink>());

  const Defer reset_logger {[] { get_logger().reset(); }};

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace
}  // namespace tactile::tests

auto main(const int argc, char* argv[]) -> int
{
  return tactile::tests::run(argc, argv);
}
