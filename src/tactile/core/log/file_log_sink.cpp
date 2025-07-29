// Copyright (C) 2025 Albin Johansson

module tactile.core.log;

import tactile.core.ext.std;
import tactile.core.prelude;

namespace tactile {
namespace {

class FileLogSink final : public ILogSink
{
 public:
  explicit FileLogSink(const Path& log_file)
    : m_stream {log_file, std::ios::out | std::ios::trunc}
  {
    if (!m_stream.good()) {
      throw std::runtime_error {"could not create log file"};
    }
  }

  void log(const LogMessage& msg) override
  {
    m_stream << msg.prefix << ' ' << msg.text << '\n';
  }

  void flush() override
  {
    m_stream.flush();
  }

 private:
  std::ofstream m_stream;
};

}  // namespace

auto make_file_log_sink(const Path& log_file) -> Unique<ILogSink>
{
  return make_unique<FileLogSink>(log_file);
}

}  // namespace tactile
