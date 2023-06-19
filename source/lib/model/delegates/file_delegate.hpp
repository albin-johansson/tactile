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

#include "common/type/dispatcher.hpp"
#include "model/event/file_events.hpp"
#include "model/model.hpp"

namespace tactile {

void on_open_document(Model& model, const OpenDocumentEvent& event);

void on_close_document(Model& model, const CloseDocumentEvent& event);

void on_select_document(Model& model, const SelectDocumentEvent& event);

void on_save(Model& model, Dispatcher& dispatcher, const SaveEvent& event);

void on_save_as(Model& model, Dispatcher& dispatcher, const SaveAsEvent& event);

void on_show_save_as_dialog(Model& model,
                            Dispatcher& dispatcher,
                            const ShowSaveAsDialogEvent& event);

void on_reopen_last_closed_file(Model& model,
                                Dispatcher& dispatcher,
                                const ReopenLastClosedFileEvent& event);

void on_clear_file_history(Model& model, const ClearFileHistoryEvent& event);

}  // namespace tactile
