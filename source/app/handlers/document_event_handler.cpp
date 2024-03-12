// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <entt/signal/dispatcher.hpp>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/util/fmt.hpp"
#include "handlers/event_handlers.hpp"
#include "io/export/map_exporter.hpp"
#include "model/document/map_document.hpp"
#include "model/event/document_events.hpp"
#include "model/file_history.hpp"
#include "model/model.hpp"
#include "runtime/app_context.hpp"
#include "ui/dialog/save_as_dialog.hpp"

namespace tactile {
namespace {

void on_open_document(const OpenDocumentEvent& event)
{
  spdlog::trace("OpenDocumentEvent(document_id: {})", event.document_id);
  get_model().open_document(event.document_id);
}

void on_close_document(const CloseDocumentEvent& event)
{
  spdlog::trace("CloseDocumentEvent(document_id: {})", event.document_id);

  auto& model = get_model();
  const auto document = model.get_document_ptr(event.document_id);

  if (document->is_map() && document->has_path()) {
    set_last_closed_file(document->get_path());
  }

  model.close_document(event.document_id);
}

void on_select_document(const SelectDocumentEvent& event)
{
  spdlog::trace("SelectDocumentEvent(document_id: {})", event.document_id);
  get_model().select_document(event.document_id);
}

void on_open_save_as_dialog(const OpenSaveAsDialogEvent&)
{
  spdlog::trace("OpenSaveAsDialogEvent");
  if (get_model().has_active_document()) {
    ui::show_save_as_dialog(get_dispatcher());
  }
}

void on_save(const SaveEvent&)
{
  spdlog::trace("SaveEvent");

  // TODO ability to save tileset documents
  if (auto* document = get_model().active_map_document()) {
    if (document->has_path()) {
      save_map_document_to_disk(*document);

      document->get_history().mark_as_clean();
      document->set_name(document->get_path().filename().string());
    }
    else {
      on_open_save_as_dialog(OpenSaveAsDialogEvent {});
    }
  }
}

void on_save_as(const SaveAsEvent& event)
{
  spdlog::trace("SaveAsEvent(path: {})", event.path);

  if (auto* document = get_model().active_document()) {
    document->set_path(event.path);
    on_save(SaveEvent {});
  }
}

}  // namespace

void install_document_event_handler()
{
  auto& dispatcher = get_dispatcher();

  dispatcher.sink<OpenDocumentEvent>().connect<&on_open_document>();
  dispatcher.sink<CloseDocumentEvent>().connect<&on_close_document>();
  dispatcher.sink<SelectDocumentEvent>().connect<&on_select_document>();

  dispatcher.sink<SaveEvent>().connect<&on_save>();
  dispatcher.sink<SaveAsEvent>().connect<&on_save_as>();
  dispatcher.sink<OpenSaveAsDialogEvent>().connect<&on_open_save_as_dialog>();
}

}  // namespace tactile
