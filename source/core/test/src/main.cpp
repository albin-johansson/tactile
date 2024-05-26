// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <memory>   // make_unique
#include <utility>  // move

#include <gtest/gtest.h>

#include "tactile/base/util/chrono.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/terminal_log_sink.hpp"
#include "tactile/core/platform/win32.hpp"
#include "tactile/core/util/scope_guard.hpp"

using namespace tactile;

auto main(int argc, char* argv[]) -> int
{
  win32_enable_virtual_terminal_processing();

  auto terminal_sink = std::make_unique<TerminalLogSink>();
  terminal_sink->use_ansi_colors(true);

  Logger logger {};
  logger.add_sink(std::move(terminal_sink));
  logger.set_min_level(LogLevel::kTrace);
  logger.set_reference_instant(SteadyClock::now());

  const ScopeGuard logger_guard {[] { set_default_logger(nullptr); }};
  set_default_logger(&logger);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
