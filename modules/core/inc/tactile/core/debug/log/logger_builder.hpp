// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/debug/log/log_level.hpp"
#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Builder class for constructing `Logger` instances.
 */
class TACTILE_CORE_API LoggerBuilder final {
 public:
  using Self = LoggerBuilder;

  /**
   * \brief Sets the initial minimum log level of log messages.
   *
   * \param level the minimum log level.
   *
   * \return the builder instance.
   */
  auto min_level(LogLevel level) -> Self&;

  /**
   * \brief Sets the minimum level of messages that would trigger a flush.
   *
   * \param level the minimum flush log level.
   *
   * \return the builder instance.
   */
  auto flush_on(LogLevel level) -> Self&;

  /**
   * \brief Uses the time of the logger initialization as the reference timepoint.
   *
   * \return the builder instance.
   */
  auto use_initialization_time_as_reference_instant() -> Self&;

  /**
   * \brief Enables colored terminal output.
   *
   * \note This function only has an effect if the terminal sink is included.
   *
   * \return the builder instance.
   */
  auto use_colored_terminal_output() -> Self&;

  /**
   * \brief Includes a terminal sink with the logger.
   *
   * \return the builder instance.
   */
  auto with_terminal_sink() -> Self&;

  /**
   * \brief Includes a log file sink with the logger.
   *
   * \return the builder instance.
   */
  auto with_file_sink(FilePath file_path) -> Self&;

  /**
   * \brief Constructs the described logger.
   *
   * \return a logger instance.
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
