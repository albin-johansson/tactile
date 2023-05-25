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

#include "file_delegate.hpp"

#include "cmd/command_stack.hpp"
#include "components/document.hpp"
#include "components/file_history.hpp"
#include "io/export/map_exporter.hpp"
#include "model/systems/document_system.hpp"
#include "ui/dialog/save_as_dialog.hpp"

namespace tactile {

void on_open_document(Model& model, const OpenDocumentEvent& event)
{
  sys::open_document(model, event.document);
}

void on_close_document(Model& model, const CloseDocumentEvent& event)
{
  const auto& document = model.get<Document>(event.document);

  if (document.type == DocumentType::Map && document.path.has_value()) {
    auto& file_history = model.get<FileHistory>();
    file_history.last_closed_file = document.path->string();
  }

  sys::close_document(model, event.document);
}

void on_select_document(Model& model, const SelectDocumentEvent& event)
{
  sys::select_document(model, event.document);
}

void on_save(Model& model, Dispatcher& dispatcher, const SaveEvent&)
{
  if (!sys::is_map_document_active(model)) {
    return;
  }

  const auto document_entity = sys::get_active_document(model);
  const auto& document = model.get<Document>(document_entity);

  if (document.path.has_value()) {
    save_map_document_to_disk(model, document_entity);

    auto& commands = model.get<CommandStack>(document_entity);
    commands.mark_as_clean();

    const auto document_name = document.path->filename().string();
    sys::set_document_name(model, document_entity, document_name);
  }
  else {
    dispatcher.trigger(ShowSaveAsDialogEvent {});
  }
}

void on_save_as(Model& model, Dispatcher& dispatcher, const SaveAsEvent& event)
{
  if (sys::is_map_document_active(model)) {
    const auto document_entity = sys::get_active_document(model);

    auto& document = model.get<Document>(document_entity);
    document.path = event.path;

    dispatcher.trigger(SaveEvent {});
  }
}

void on_show_save_as_dialog(Model& model,
                            Dispatcher& dispatcher,
                            const ShowSaveAsDialogEvent&)
{
  if (sys::is_map_document_active(model)) {
    ui::show_save_as_dialog(model, dispatcher);
  }
}

}  // namespace tactile
