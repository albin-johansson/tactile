// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/log/file_logger_sink.hpp"

#include <ios>  // ios

#include "tactile/foundation/debug/error.hpp"

namespace tactile {

FileLoggerSink::FileLoggerSink(const FilePath& log_file)
  : mStream {log_file, std::ios::out | std::ios::trunc}
{
  if (!mStream.good()) {
    throw RuntimeError {"Could not open file sink output stream"};
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
