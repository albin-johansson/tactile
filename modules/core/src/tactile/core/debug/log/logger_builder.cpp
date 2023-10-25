// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/log/logger_builder.hpp"

#include <utility>  // move

#include "tactile/core/debug/log/file_logger_sink.hpp"
#include "tactile/core/debug/log/terminal_logger_sink.hpp"

namespace tactile {

auto LoggerBuilder::min_level(const LogLevel level) -> Self&
{
  mMinLevel = level;
  return *this;
}

auto LoggerBuilder::flush_on(const LogLevel level) -> Self&
{
  mFlushLevel = level;
  return *this;
}

auto LoggerBuilder::use_initialization_time_as_reference_instant() -> Self&
{
  mUseStartupTimeAsReferenceInstant = true;
  return *this;
}

auto LoggerBuilder::use_colored_terminal_output() -> Self&
{
  mUseColoredTerminalOutput = true;
  return *this;
}

auto LoggerBuilder::with_terminal_sink() -> Self&
{
  mUseTerminalSink = true;
  return *this;
}

auto LoggerBuilder::with_file_sink(FilePath file_path) -> Self&
{
  mLogFilePath = std::move(file_path);
  return *this;
}

auto LoggerBuilder::build() -> Logger
{
  Logger logger;

  if (mUseStartupTimeAsReferenceInstant) {
    logger.set_reference_instant(SteadyClock::now());
  }

  logger.set_min_level(mMinLevel);
  logger.flush_on(mFlushLevel);

  if (mUseTerminalSink) {
    auto terminal_sink = make_managed<TerminalLoggerSink>();
    terminal_sink->use_ansi_colors(mUseColoredTerminalOutput);

    logger.add_sink(managed_cast<ILoggerSink>(std::move(terminal_sink)));
  }

  if (mLogFilePath.has_value()) {
    auto file_sink = make_managed<FileLoggerSink>(std::move(*mLogFilePath));
    logger.add_sink(managed_cast<ILoggerSink>(std::move(file_sink)));
  }

  return logger;
}

}  // namespace tactile
