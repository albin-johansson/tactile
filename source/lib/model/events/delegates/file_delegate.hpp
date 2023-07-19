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
#include "model/events/file_events.hpp"
#include "model/registry.hpp"

namespace tactile {

void on_open_document(Registry& registry, const OpenDocumentEvent& event);

void on_close_document(Registry& registry, const CloseDocumentEvent& event);

void on_select_document(Registry& registry, const SelectDocumentEvent& event);

void on_save(Registry& registry, Dispatcher& dispatcher, const SaveEvent& event);

void on_save_as(Registry& registry, Dispatcher& dispatcher, const SaveAsEvent& event);

void on_reopen_last_closed_file(Registry& registry,
                                Dispatcher& dispatcher,
                                const ReopenLastClosedFileEvent& event);

void on_clear_file_history(Registry& registry, const ClearFileHistoryEvent& event);

}  // namespace tactile
