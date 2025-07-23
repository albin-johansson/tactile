// Copyright (C) 2025 Albin Johansson

export module tactile.core.log:file_log_sink;

export import tactile.core.ext.std;
export import :sink;

export namespace tactile {

/// A log sink that writes messages to a file.
class FileLogSink final : public ILogSink
{
 public:
  explicit FileLogSink(const Path& log_file);

  void log(const LogMessage& msg) override;

  void flush() override;

 private:
  std::ofstream m_stream;
};

}  // namespace tactile
