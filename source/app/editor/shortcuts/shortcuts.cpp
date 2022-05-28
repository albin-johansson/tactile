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

#include <vector>  // vector

#include <centurion/event.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/common/memory.hpp"
#include "editor/shortcuts/edit_shortcuts.hpp"
#include "editor/shortcuts/file_shortcuts.hpp"
#include "editor/shortcuts/view_shortcuts.hpp"

namespace tactile {
namespace {

inline std::vector<Unique<AShortcut>> _shortcuts;

template <typename T>
void _load_shortcut()
{
  _shortcuts.push_back(std::make_unique<T>());
}

}  // namespace

void load_default_shortcuts()
{
  // File
  _load_shortcut<NewMapShortcut>();
  _load_shortcut<OpenMapShortcut>();
  _load_shortcut<SaveShortcut>();
  _load_shortcut<SaveAsShortcut>();

  // Edit
  _load_shortcut<UndoShortcut>();
  _load_shortcut<RedoShortcut>();

  _load_shortcut<EnableStampShortcut>();
  _load_shortcut<EnableEraserShortcut>();
  _load_shortcut<EnableBucketShortcut>();
  _load_shortcut<EnableObjectSelectionShortcut>();
  _load_shortcut<EnableRectangleToolShortcut>();
  _load_shortcut<EnableEllipseToolShortcut>();
  _load_shortcut<EnablePointToolShortcut>();

  _load_shortcut<OpenComponentEditorShortcut>();
  _load_shortcut<OpenSettingsShortcut>();

  // View
  _load_shortcut<CenterViewportShortcut>();
  _load_shortcut<IncreaseViewportZoomShortcut>();
  _load_shortcut<DecreaseViewportZoomShortcut>();

  _load_shortcut<IncreaseFontSizeShortcut>();
  _load_shortcut<DecreaseFontSizeShortcut>();

  _load_shortcut<ToggleGridShortcut>();
  _load_shortcut<ToggleLayerHighlightShortcut>();
  _load_shortcut<ToggleUiShortcut>();

  _load_shortcut<PanRightShortcut>();
  _load_shortcut<PanLeftShortcut>();
  _load_shortcut<PanUpShortcut>();
  _load_shortcut<PanDownShortcut>();

  // Map
  _load_shortcut<AddTilesetShortcut>();

  _load_shortcut<AddRowShortcut>();
  _load_shortcut<AddColumnShortcut>();
  _load_shortcut<RemoveRowShortcut>();
  _load_shortcut<RemoveColumnShortcut>();
}

void update_shortcuts(const DocumentModel& model,
                      const cen::keyboard_event& event,
                      entt::dispatcher& dispatcher)
{
  for (const auto& shortcut : _shortcuts) {
    shortcut->poll(model, event, dispatcher);
  }
}

}  // namespace tactile
