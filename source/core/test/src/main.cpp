// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <chrono>   // steady_clock
#include <memory>   // make_unique
#include <utility>  // move

#include <gtest/gtest.h>

#include "tactile/base/util/scope_exit.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/terminal_log_sink.hpp"
#include "tactile/core/platform/win32.hpp"

auto main(int argc, char* argv[]) -> int
{
  tactile::core::win32_enable_virtual_terminal_processing();

  auto terminal_sink = std::make_unique<tactile::core::TerminalLogSink>();
  terminal_sink->use_ansi_colors(true);

  tactile::core::Logger logger {};
  logger.add_sink(std::move(terminal_sink));
  logger.set_min_level(tactile::LogLevel::kTrace);
  logger.set_reference_instant(std::chrono::steady_clock::now());

  const tactile::ScopeExit logger_guard {[] { tactile::core::set_default_logger(nullptr); }};
  set_default_logger(&logger);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
