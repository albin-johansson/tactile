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

#include <entt/signal/dispatcher.hpp>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/util/fmt.hpp"
#include "handlers/event_handlers.hpp"
#include "io/map/emit/emitter.hpp"
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
