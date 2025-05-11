// Copyright (C) 2025 Albin Johansson

export module tactile.log:console_log_sink;

export import :sink;

export namespace tactile {

/// A log sink that writes messages to the console.
class ConsoleLogSink final : public ILogSink
{
 public:
  void log(const LogMessage& msg) override;

  void flush() override;

  void set_use_colors(bool use_colors);

 private:
  bool m_use_colors {true};
};

}  // namespace tactile
