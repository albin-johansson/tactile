// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/log/log_sink.hpp"

namespace tactile {

/// A log sink that forwards messages to the console.
class ConsoleLogSink final : public ILogSink
{
 public:
  void flush() override;

  void log(const LogEntry& entry) override;

  void set_ansi_colors(bool enabled);

 private:
  bool m_ansi_colors {true};
};

}  // namespace tactile
