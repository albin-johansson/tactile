/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "document_event_handler.hpp"

#include <entt/signal/dispatcher.hpp>

#include "editor/app_context.hpp"
#include "editor/ui/dialog/save_as_dialog.hpp"
#include "io/map/emit/emitter.hpp"
#include "io/proto/history.hpp"
#include "model/document/map_document.hpp"
#include "model/event/document_events.hpp"
#include "model/model.hpp"

namespace tactile {
namespace {

void on_open_document(const OpenDocumentEvent& event)
{
  get_model().open_document(event.document_id);
}

void on_close_document(const CloseDocumentEvent& event)
{
  auto& model = get_model();
  const auto document = model.get_document(event.id);

  if (document->is_map() && document->has_path()) {
    io::set_last_closed_file(document->get_path());
  }

  model.close_document(event.id);
}

void on_select_document(const SelectDocumentEvent& event)
{
  get_model().select_document(event.id);
}

void on_open_save_as_dialog()
{
  if (get_model().has_active_document()) {
    ui::show_save_as_dialog(get_dispatcher());
  }
}

void on_save()
{
  // TODO ability to save tileset documents
  if (auto* document = get_model().active_map()) {
    if (document->has_path()) {
      io::emit_map(*document);

      document->get_history().mark_as_clean();
      document->set_name(document->get_path().filename().string());
    }
    else {
      on_open_save_as_dialog();
    }
  }
}

void on_save_as(const SaveAsEvent& event)
{
  if (auto* document = get_model().active_document()) {
    document->set_path(event.path);
    on_save();
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
