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
#include "io/export/map_exporter.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/persistence/file_history_components.hpp"
#include "ui/dialog/save_as_dialog.hpp"

namespace tactile {

void on_open_document(Registry& registry, const OpenDocumentEvent& event)
{
  sys::open_document(registry, event.document);
}

void on_close_document(Registry& registry, const CloseDocumentEvent& event)
{
  const auto& document = registry.get<Document>(event.document);

  if (document.type == DocumentType::Map && document.path.has_value()) {
    auto& file_history = registry.get<FileHistory>();
    file_history.last_closed_file = document.path->string();
  }

  sys::close_document(registry, event.document);
}

void on_select_document(Registry& registry, const SelectDocumentEvent& event)
{
  sys::select_document(registry, event.document);
}

void on_save(Registry& registry, Dispatcher& dispatcher, const SaveEvent&)
{
  if (!sys::is_map_document_active(registry)) {
    return;
  }

  const auto document_entity = sys::get_active_document(registry);
  const auto& document = registry.get<Document>(document_entity);

  if (document.path.has_value()) {
    save_map_document_to_disk(registry, document_entity);

    auto& commands = registry.get<CommandStack>(document_entity);
    commands.mark_as_clean();

    const auto document_name = document.path->filename().string();
    sys::set_document_name(registry, document_entity, document_name);
  }
  else {
    dispatcher.trigger(ShowSaveAsDialogEvent {});
  }
}

void on_save_as(Registry& registry, Dispatcher& dispatcher, const SaveAsEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    const auto document_entity = sys::get_active_document(registry);

    auto& document = registry.get<Document>(document_entity);
    document.path = event.path;

    dispatcher.trigger(SaveEvent {});
  }
}

void on_show_save_as_dialog(Registry& registry,
                            Dispatcher& dispatcher,
                            const ShowSaveAsDialogEvent&)
{
  if (sys::is_map_document_active(registry)) {
    show_save_as_dialog(ModelView {registry, dispatcher});
  }
}

void on_reopen_last_closed_file(Registry& registry,
                                Dispatcher& dispatcher,
                                const ReopenLastClosedFileEvent&)
{
  // TODO update if tileset documents viewing needs to be supported
  const auto& file_history = registry.get<FileHistory>();
  Path file_path {file_history.last_closed_file.value()};
  dispatcher.enqueue<OpenMapEvent>(std::move(file_path));
}

void on_clear_file_history(Registry& registry, const ClearFileHistoryEvent&)
{
  auto& file_history = registry.get<FileHistory>();
  file_history.entries.clear();
}

}  // namespace tactile
