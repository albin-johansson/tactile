// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <memory>   // make_unique
#include <utility>  // move

#include <gtest/gtest.h>

#include "tactile/base/util/chrono.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/terminal_log_sink.hpp"
#include "tactile/core/platform/win32.hpp"
#include "tactile/core/util/scope_guard.hpp"

auto main(int argc, char* argv[]) -> int
{
  tactile::core::win32_enable_virtual_terminal_processing();

  auto terminal_sink = std::make_unique<tactile::core::TerminalLogSink>();
  terminal_sink->use_ansi_colors(true);

  tactile::core::Logger logger {};
  logger.add_sink(std::move(terminal_sink));
  logger.set_min_level(tactile::LogLevel::kTrace);
  logger.set_reference_instant(tactile::SteadyClock::now());

  const tactile::core::ScopeGuard logger_guard {
    [] { tactile::core::set_default_logger(nullptr); }};
  set_default_logger(&logger);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
