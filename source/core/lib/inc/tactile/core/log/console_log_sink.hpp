// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/log/log_sink.hpp"

namespace tactile {

/// A log sink that simply forwards messages to stdout.
class TACTILE_CORE_API ConsoleLogSink final : public ILogSink
{
 public:
  void log(const LogMessage& msg) override;

  void flush() override;
};

}  // namespace tactile
