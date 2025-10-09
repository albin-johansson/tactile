// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <fstream>

#include "tactile/core/container/path.hpp"
#include "tactile/core/log/log_sink.hpp"

namespace tactile {

/// A log sink that forwards messages to a file.
class FileLogSink final : public ILogSink
{
 public:
  explicit FileLogSink(const Path& path);

  void flush() override;

  void log(const LogEntry& entry) override;

 private:
  std::ofstream m_file {};
};

}  // namespace tactile
