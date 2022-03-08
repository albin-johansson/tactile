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

#include "shortcuts.hpp"

#include <memory>  // unique_ptr, make_unique
#include <vector>  // vector

#include "edit_shortcuts.hpp"
#include "file_shortcuts.hpp"
#include "view_shortcuts.hpp"

namespace tactile {
namespace {

inline std::vector<std::unique_ptr<AShortcut>> gShortcuts;

template <typename T>
void LoadShortcut()
{
  gShortcuts.push_back(std::make_unique<T>());
}

}  // namespace

void LoadDefaultShortcuts()
{
  // File
  LoadShortcut<NewMapShortcut>();
  LoadShortcut<OpenMapShortcut>();
  LoadShortcut<OpenSettingsShortcut>();
  LoadShortcut<SaveShortcut>();
  LoadShortcut<SaveAsShortcut>();

  // Edit
  LoadShortcut<UndoShortcut>();
  LoadShortcut<RedoShortcut>();

  LoadShortcut<EnableStampShortcut>();
  LoadShortcut<EnableEraserShortcut>();
  LoadShortcut<EnableBucketShortcut>();
  LoadShortcut<EnableObjectSelectionShortcut>();
  LoadShortcut<EnableRectangleToolShortcut>();

  LoadShortcut<AddRowShortcut>();
  LoadShortcut<AddColumnShortcut>();
  LoadShortcut<RemoveRowShortcut>();
  LoadShortcut<RemoveColumnShortcut>();

  LoadShortcut<AddTilesetShortcut>();

  // View
  LoadShortcut<CenterViewportShortcut>();
  LoadShortcut<IncreaseViewportZoomShortcut>();
  LoadShortcut<DecreaseViewportZoomShortcut>();

  LoadShortcut<ToggleGridShortcut>();
  LoadShortcut<ToggleUiShortcut>();

  LoadShortcut<PanRightShortcut>();
  LoadShortcut<PanLeftShortcut>();
  LoadShortcut<PanUpShortcut>();
  LoadShortcut<PanDownShortcut>();
}

void UpdateShortcuts(const document_model& model,
                     const widget_manager& widgets,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher)
{
  for (const auto& shortcut : gShortcuts) {
    shortcut->Poll(model, widgets, event, dispatcher);
  }
}

}  // namespace tactile
