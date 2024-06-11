// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/event/file_event_handler.hpp"

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/file_events.hpp"
#include "tactile/core/log/logger.hpp"

namespace tactile {

FileEventHandler::FileEventHandler(Model* model)
  : mModel {require_not_null(model, "null model")}
{}

void FileEventHandler::install(EventDispatcher& dispatcher)
{
  using Self = FileEventHandler;

  // clang-format off
  dispatcher.bind<SaveEvent, &Self::on_save>(this);
  dispatcher.bind<SaveAsEvent, &Self::on_save_as>(this);
  dispatcher.bind<ReopenLastClosedFileEvent, &Self::on_reopen_last_closed_file>(this);
  dispatcher.bind<ClearFileHistoryEvent, &Self::on_clear_file_history>(this);
  dispatcher.bind<CloseEvent, &Self::on_close>(this);
  dispatcher.bind<QuitEvent, &Self::on_quit>(this);
  // clang-format on
}

void FileEventHandler::on_save(const SaveEvent& event)
{
  TACTILE_LOG_TRACE("SaveEvent");
}

void FileEventHandler::on_save_as(const SaveAsEvent& event)
{
  TACTILE_LOG_TRACE("SaveAsEvent");
}

void FileEventHandler::on_reopen_last_closed_file(
    const ReopenLastClosedFileEvent& event)
{
  TACTILE_LOG_TRACE("ReopenLastClosedFileEvent");
}

void FileEventHandler::on_clear_file_history(const ClearFileHistoryEvent& event)
{
  TACTILE_LOG_TRACE("ClearFileHistoryEvent");
}

void FileEventHandler::on_close(const CloseEvent& event)
{
  TACTILE_LOG_TRACE("CloseEvent");
}

void FileEventHandler::on_quit(const QuitEvent& event)
{
  TACTILE_LOG_TRACE("QuitEvent");
}

}  // namespace tactile
