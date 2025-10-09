// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/macros.hpp"

namespace tactile::editor {

/// Manages the logger used by the editor.
class LoggerContext final
{
 public:
  TACTILE_DELETE_COPY(LoggerContext);
  TACTILE_DEFAULT_MOVE(LoggerContext);

  /// Initializes and installs the logger.
  LoggerContext();

  /// Uninstalls the logger.
  ~LoggerContext() noexcept;

 private:
  Unique<Logger> m_logger {};
};

}  // namespace tactile::editor
