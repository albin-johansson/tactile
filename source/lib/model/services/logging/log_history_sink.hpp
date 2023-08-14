/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

#include "core/containers/deque.hpp"
#include "tactile/core/containers/string.hpp"
#include "model/services/logging/log_level.hpp"

namespace tactile {

struct LogEntry final {
  LogLevelBits level;
  String msg;
};

class LogHistorySink final :
    public spdlog::sinks::base_sink<spdlog::details::null_mutex> {
 public:
  void sink_it_(const spdlog::details::log_msg& msg) override;

  void flush_() override;

  void clear();

  [[nodiscard]] auto get_messages() const -> const Deque<LogEntry>& { return mMessages; }

 private:
  Deque<LogEntry> mMessages;
};

}  // namespace tactile
