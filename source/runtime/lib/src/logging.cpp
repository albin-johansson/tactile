// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/runtime/logging.hpp"

#include "tactile/base/container/buffer.hpp"
#include "tactile/core/log/logger.hpp"

namespace tactile::runtime::internal {

void log(const LogLevel level, const std::string_view fmt, const std::format_args args)
{
  Buffer<char, 256> buffer;  // NOLINT uninitialized
  vformat_to_buffer(buffer, fmt, args);
  TACTILE_LOG(level, "{}", buffer.view());
}

}  // namespace tactile::runtime::internal
