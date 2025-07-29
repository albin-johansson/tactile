// Copyright (C) 2025 Albin Johansson

module tactile.core.log;

import tactile.core.ext.std;
import tactile.core.prelude;

namespace tactile {
namespace {

constexpr StringView kAnsiColorReset = "\x1B[0m";
constexpr StringView kAnsiColorFgRedBold = "\x1B[31m\x1B[1m";
constexpr StringView kAnsiColorFgYellow = "\x1B[33m";
constexpr StringView kAnsiColorFgMagenta = "\x1B[35m";
constexpr StringView kAnsiColorFgCyan = "\x1B[36m";
constexpr StringView kAnsiColorFgWhite = "\x1B[37m";

[[nodiscard]]
auto _get_ansi_color(const LogLevel level) noexcept -> StringView
{
  switch (level) {
    case LogLevel::kTrace:
      return kAnsiColorFgMagenta;
    case LogLevel::kDebug:
      return kAnsiColorFgCyan;
    case LogLevel::kInfo:
      return kAnsiColorFgWhite;
    case LogLevel::kWarn:
      return kAnsiColorFgYellow;
    case LogLevel::kError:
      return kAnsiColorFgRedBold;
  }

  return kAnsiColorFgWhite;
}

class ConsoleLogSink final : public ILogSink
{
 public:
  explicit ConsoleLogSink(const bool use_colors)
    : m_use_colors {use_colors}
  {}

  void log(const LogMessage& msg) override
  {
    if (m_use_colors) {
      std::clog << _get_ansi_color(msg.level);
    }

    std::clog << msg.prefix << ' ' << msg.text;

    if (m_use_colors) {
      std::clog << kAnsiColorReset;
    }

    std::clog << '\n';
  }

  void flush() override
  {
    std::clog.flush();
  }

 private:
  bool m_use_colors;
};

}  // namespace

auto make_console_log_sink(const bool use_colors) -> Unique<ILogSink>
{
  return make_unique<ConsoleLogSink>(use_colors);
}

}  // namespace tactile
