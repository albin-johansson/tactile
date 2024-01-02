// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/log/log_level.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Builder class for constructing `Logger` instances.
 */
class TACTILE_FOUNDATION_API LoggerBuilder final {
 public:
  using Self = LoggerBuilder;

  /**
   * Sets the initial minimum log level of log messages.
   *
   * \param level The minimum log level.
   *
   * \return
   *    The builder instance.
   */
  auto min_level(LogLevel level) -> Self&;

  /**
   * Sets the minimum level of messages that would trigger a flush.
   *
   * \param level The minimum flush log level.
   *
   * \return
   *    The builder instance.
   */
  auto flush_on(LogLevel level) -> Self&;

  /**
   * Uses the time of the logger initialization as the reference timepoint.
   *
   * \return
   *    The builder instance.
   */
  auto use_initialization_time_as_reference_instant() -> Self&;

  /**
   * Enables colored terminal output.
   *
   * \note This function only has an effect if the terminal sink is included.
   *
   * \return
   *    The builder instance.
   */
  auto use_colored_terminal_output() -> Self&;

  /**
   * Includes a terminal sink with the logger.
   *
   * \return
   *    The builder instance.
   */
  auto with_terminal_sink() -> Self&;

  /**
   * Includes a log file sink with the logger.
   *
   * \return
   *    The builder instance.
   */
  auto with_file_sink(FilePath file_path) -> Self&;

  /**
   * Constructs the described logger.
   *
   * \return
   *    A logger instance.
   */
  auto build() -> Logger;

 private:
  LogLevel mMinLevel {LogLevel::kTrace};
  LogLevel mFlushLevel {LogLevel::kError};
  Maybe<FilePath> mLogFilePath;
  bool mUseStartupTimeAsReferenceInstant : 1 {};
  bool mUseTerminalSink : 1 {};
  bool mUseColoredTerminalOutput : 1 {};
};

}  // namespace tactile
