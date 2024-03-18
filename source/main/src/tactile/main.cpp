// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <cstdlib>
#include <filesystem>
#include <memory>
#include <utility>

#include "tactile/base/prelude.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/terminal_log_sink.hpp"

auto main(const int, char*[]) -> int
{
  const auto binary_dir = std::filesystem::current_path();

  tactile::Logger logger {};
  logger.set_reference_instant(tactile::SteadyClock::now());
  logger.set_min_level(tactile::LogLevel::kTrace);

  auto terminal_log_sink = std::make_unique<tactile::TerminalLogSink>();
  logger.add_sink(std::move(terminal_log_sink));

  TACTILE_LOG_DEBUG("Tactile " TACTILE_VERSION_STRING);
  return EXIT_SUCCESS;
}
