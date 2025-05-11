// Copyright (C) 2025 Albin Johansson

module tactile.log;

namespace tactile {

FileLogSink::FileLogSink(const Path& log_file)
  : m_stream {log_file, std::ios::out | std::ios::trunc}
{
  if (!m_stream.good()) {
    throw std::runtime_error {"could not create log file"};
  }
}

void FileLogSink::log(const LogMessage& msg)
{
  m_stream << msg.prefix << ' ' << msg.text << '\n';
}

void FileLogSink::flush()
{
  m_stream.flush();
}

}  // namespace tactile
