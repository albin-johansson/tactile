// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/log/file_log_sink.hpp"

#include <ios>
#include <ostream>
#include <stdexcept>

namespace tactile {

FileLogSink::FileLogSink(const Path& path)
  : m_file {path, std::ios::out | std::ios::trunc}
{
  if (!m_file.good()) {
    throw std::runtime_error {"[FileLogSink]: could not create file"};
  }
}

void FileLogSink::flush()
{
  m_file << std::flush;
}

void FileLogSink::log(const LogEntry& entry)
{
  m_file << entry.prefix << ' ' << entry.message << '\n';
}

}  // namespace tactile
