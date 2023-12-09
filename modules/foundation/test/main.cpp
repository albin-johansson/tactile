// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

//#include "tactile/foundation/debug/log/logger_builder.hpp"
//#include "tactile/foundation/math/rng.hpp"
//#include "tactile/foundation/misc/scope_guard.hpp"

//using namespace tactile;

auto main(int argc, char* argv[]) -> int
{
  //  auto logger = LoggerBuilder {}
  //                    .use_initialization_time_as_reference_instant()
  //                    .min_level(LogLevel::kDebug)
  //                    .flush_on(LogLevel::kError)
  //                    .with_file_sink("core_test_log.txt")
  //                    .with_terminal_sink()
  //                    .use_colored_terminal_output()
  //                    .build();

  //  set_default_logger(&logger);
  //  const ScopeGuard logger_guard {[] { set_default_logger(nullptr); }};
  //
  //  rng_init();

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}