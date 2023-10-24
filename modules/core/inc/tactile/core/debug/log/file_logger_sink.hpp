// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <fstream>  // ofstream

#include "tactile/core/api.hpp"
#include "tactile/core/debug/log/logger_sink.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief A logger sink that forwards log messages to a text file.
 */
class TACTILE_CORE_API FileLoggerSink final : public ILoggerSink {
 public:
  /**
   * \brief Creates a file sink that writes to the specified log file.
   *
   * \param log_file the log file destination.
   */
  explicit FileLoggerSink(const FilePath& log_file);

  void log(const LogMessage& msg) override;

  void flush() override;

 private:
  std::ofstream mStream;
};

}  // namespace tactile
