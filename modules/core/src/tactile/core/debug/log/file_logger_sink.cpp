// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/log/file_logger_sink.hpp"

#include <ios>  // ios

#include "tactile/core/debug/error.hpp"

namespace tactile {

FileLoggerSink::FileLoggerSink(const fs::Path& log_file)
  : mStream {log_file, std::ios::out | std::ios::trunc}
{
  if (!mStream.good()) {
    throw Error {"Could not open file sink output stream"};
  }
}

void FileLoggerSink::log(const LogMessage& msg)
{
  mStream << msg.prefix << ' ' << msg.text << '\n';
}

void FileLoggerSink::flush()
{
  mStream.flush();
}

}  // namespace tactile
