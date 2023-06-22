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

#include "common/type/ecs.hpp"
#include "common/type/path.hpp"

namespace tactile {

/// Event used to open an existing document in its own tab.
struct OpenDocumentEvent final {
  Entity document {kNullEntity};  ///< The document that should be opened.
};

/// Event used to close an open document.
struct CloseDocumentEvent final {
  Entity document {kNullEntity};  ///< The document that should be closed.
};

/// Event used to make an open document active.
struct SelectDocumentEvent final {
  Entity document {kNullEntity};  ///< The document that should be selected.
};

/// Event used to save the active document to disk.
struct SaveEvent final {};

/// Event used to save the active document to a specific path on disk.
struct SaveAsEvent final {
  Path path;
};

/// Event used to open the 'Save As...' dialog.
struct ShowSaveAsDialogEvent final {};

struct ReopenLastClosedFileEvent final {};

struct ClearFileHistoryEvent final {};

/// Event used to stop running the application.
struct QuitEvent final {};

}  // namespace tactile