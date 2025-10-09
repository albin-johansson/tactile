// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/editor/util/logger_context.hpp"

#include "tactile/core/log/console_log_sink.hpp"
#include "tactile/core/log/file_log_sink.hpp"

namespace tactile::editor {

LoggerContext::LoggerContext()
  : m_logger {make_unique<Logger>()}
{
  m_logger->set_ref_instant(Logger::clock_type::now());
  m_logger->set_min_category(LogCategory::kInfo);
  m_logger->set_flush_category(LogCategory::kError);
  m_logger->add_sink(make_unique<ConsoleLogSink>());
  m_logger->add_sink(make_unique<FileLogSink>("tactile.log"));

  set_logger(m_logger.get());
  log_verbose("Initialized logger");
}

LoggerContext::~LoggerContext() noexcept
{
  log_verbose("Destroying logger");
  set_logger(nullptr);
}

}  // namespace tactile::editor
