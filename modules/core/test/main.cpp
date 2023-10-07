// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/debug/log/terminal_logger_sink.hpp"
#include "tactile/core/math/rng.hpp"
#include "tactile/core/misc/scope_guard.hpp"

using namespace tactile;

auto main(int argc, char* argv[]) -> int
{
  Logger logger;
  logger.set_reference_instant(SteadyClock::now());
  logger.set_min_level(LogLevel::kTrace);
  logger.flush_on(LogLevel::kError);

  TerminalLoggerSink terminal_logger_sink;
  terminal_logger_sink.use_ansi_colors(true);
  logger.add_sink(&terminal_logger_sink);

  set_default_logger(&logger);
  const ScopeGuard logger_guard {[] { set_default_logger(nullptr); }};

  rng_init();

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}