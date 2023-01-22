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

#include <centurion/event.hpp>
#include <entt/signal/dispatcher.hpp>

#include "common/type/ptr.hpp"
#include "common/type/vec.hpp"
#include "ui/shortcut/edit_shortcuts.hpp"
#include "ui/shortcut/file_shortcuts.hpp"
#include "ui/shortcut/view_shortcuts.hpp"

namespace tactile {
namespace {

inline Vec<Unique<Shortcut>> gShortcuts;

template <typename T>
void load_shortcut()
{
  gShortcuts.push_back(std::make_unique<T>());
}

}  // namespace

void load_default_shortcuts()
{
  // File
  load_shortcut<NewMapShortcut>();
  load_shortcut<OpenMapShortcut>();
  load_shortcut<SaveShortcut>();
  load_shortcut<SaveAsShortcut>();

  // Edit
  load_shortcut<UndoShortcut>();
  load_shortcut<RedoShortcut>();

  load_shortcut<EnableStampShortcut>();
  load_shortcut<EnableEraserShortcut>();
  load_shortcut<EnableBucketShortcut>();
  load_shortcut<EnableObjectSelectionShortcut>();
  load_shortcut<EnableRectangleToolShortcut>();
  load_shortcut<EnableEllipseToolShortcut>();
  load_shortcut<EnablePointToolShortcut>();

  load_shortcut<OpenComponentEditorShortcut>();
  load_shortcut<OpenSettingsShortcut>();

  // View
  load_shortcut<CenterViewportShortcut>();
  load_shortcut<IncreaseViewportZoomShortcut>();
  load_shortcut<DecreaseViewportZoomShortcut>();

  load_shortcut<IncreaseFontSizeShortcut>();
  load_shortcut<DecreaseFontSizeShortcut>();

  load_shortcut<ToggleGridShortcut>();
  load_shortcut<ToggleLayerHighlightShortcut>();
  load_shortcut<ToggleUiShortcut>();

  load_shortcut<PanRightShortcut>();
  load_shortcut<PanLeftShortcut>();
  load_shortcut<PanUpShortcut>();
  load_shortcut<PanDownShortcut>();

  // Map
  load_shortcut<AddTilesetShortcut>();

  load_shortcut<AddRowShortcut>();
  load_shortcut<AddColumnShortcut>();
  load_shortcut<RemoveRowShortcut>();
  load_shortcut<RemoveColumnShortcut>();
}

void update_shortcuts(const DocumentModel& model,
                      const cen::keyboard_event& event,
                      entt::dispatcher& dispatcher)
{
  for (const auto& shortcut: gShortcuts) {
    shortcut->poll(model, event, dispatcher);
  }
}

}  // namespace tactile
