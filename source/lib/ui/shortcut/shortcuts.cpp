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

#include "shortcuts.hpp"

#include <centurion/event.hpp>
#include <entt/signal/dispatcher.hpp>

#include "model/event/all.hpp"
#include "tactile/core/type/vector.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/shortcut/shortcut.hpp"

namespace tactile {
namespace {

inline Vector<Shortcut> gShortcuts;

template <typename Event>
void init_shortcut(const MenuAction action,
                   const cen::scan_code key,
                   const cen::key_mod modifiers = cen::key_mod::none)
{
  gShortcuts.push_back(Shortcut {
      .action = action,
      .key = key,
      .modifiers = modifiers,
      .activate = [](entt::dispatcher& dispatcher) { dispatcher.enqueue<Event>(); },
  });
}

void init_tool_shortcut(const ToolType tool,
                        const MenuAction action,
                        const cen::scan_code key)
{
  gShortcuts.push_back(Shortcut {.action = action,
                                 .key = key,
                                 .modifiers = cen::key_mod::none,
                                 .activate = [=](entt::dispatcher& dispatcher) {
                                   dispatcher.enqueue<SelectToolEvent>(tool);
                                 }});
}

void init_default_file_shortcuts()
{
  init_shortcut<ShowNewMapDialogEvent>(MenuAction::NewMap,
                                       cen::scancodes::n,
                                       kPrimaryModifier);
  init_shortcut<ShowOpenMapDialogEvent>(MenuAction::OpenMap,
                                        cen::scancodes::o,
                                        kPrimaryModifier);
  init_shortcut<SaveEvent>(MenuAction::Save, cen::scancodes::s, kPrimaryModifier);
  init_shortcut<OpenSaveAsDialogEvent>(MenuAction::SaveAs,
                                       cen::scancodes::s,
                                       kPrimaryModifierAndShift);
}

void init_default_edit_shortcuts()
{
  init_shortcut<UndoEvent>(MenuAction::Undo, cen::scancodes::z, kPrimaryModifier);
  init_shortcut<RedoEvent>(MenuAction::Redo, cen::scancodes::y, kPrimaryModifier);

  init_tool_shortcut(ToolType::Stamp, MenuAction::StampTool, cen::scancodes::s);
  init_tool_shortcut(ToolType::Eraser, MenuAction::EraserTool, cen::scancodes::e);
  init_tool_shortcut(ToolType::Bucket, MenuAction::BucketTool, cen::scancodes::b);
  init_tool_shortcut(ToolType::ObjectSelection,
                     MenuAction::ObjectSelectionTool,
                     cen::scancodes::q);
  init_tool_shortcut(ToolType::Rectangle, MenuAction::RectangleTool, cen::scancodes::r);
  init_tool_shortcut(ToolType::Ellipse, MenuAction::EllipseTool, cen::scancodes::t);
  init_tool_shortcut(ToolType::Point, MenuAction::PointTool, cen::scancodes::y);

  init_shortcut<OpenComponentEditorEvent>(MenuAction::ComponentEditor,
                                          cen::scancodes::c,
                                          kPrimaryModifierAndShift);
  init_shortcut<ShowSettingsEvent>(MenuAction::OpenSettings,
                                   SDL_SCANCODE_COMMA,
                                   kPrimaryModifier);
}

void init_default_view_shortcuts()
{
  const cen::scan_code plus {SDLK_PLUS};
  const cen::scan_code minus {SDLK_MINUS};

  init_shortcut<CenterViewportEvent>(MenuAction::CenterViewport,
                                     cen::scancodes::space,
                                     cen::key_mod::lshift);

  init_shortcut<IncreaseZoomEvent>(MenuAction::IncreaseZoom, plus, kPrimaryModifier);
  init_shortcut<DecreaseZoomEvent>(MenuAction::DecreaseZoom, minus, kPrimaryModifier);

  init_shortcut<IncreaseFontSizeEvent>(MenuAction::IncreaseFontSize,
                                       plus,
                                       kPrimaryModifierAndShift);
  init_shortcut<DecreaseFontSizeEvent>(MenuAction::DecreaseFontSize,
                                       minus,
                                       kPrimaryModifierAndShift);

  init_shortcut<ToggleGridEvent>(MenuAction::ToggleGrid,
                                 cen::scancodes::g,
                                 kPrimaryModifier);
  init_shortcut<ToggleHighlightLayerEvent>(MenuAction::HighlightLayer, cen::scancodes::h);
  init_shortcut<ToggleUiEvent>(MenuAction::ToggleUi, cen::scancodes::tab);

  init_shortcut<PanRightEvent>(MenuAction::PanRight,
                               cen::scancodes::right,
                               kPrimaryModifierAndShift);
  init_shortcut<PanLeftEvent>(MenuAction::PanLeft,
                              cen::scancodes::left,
                              kPrimaryModifierAndShift);
  init_shortcut<PanUpEvent>(MenuAction::PanUp,
                            cen::scancodes::up,
                            kPrimaryModifierAndShift);
  init_shortcut<PanDownEvent>(MenuAction::PanDown,
                              cen::scancodes::down,
                              kPrimaryModifierAndShift);
}

void init_default_map_shortcuts()
{
  init_shortcut<ShowTilesetCreationDialogEvent>(MenuAction::AddTileset,
                                                cen::scancodes::t,
                                                kPrimaryModifier);

  init_shortcut<AddRowEvent>(MenuAction::AddRow, cen::scancodes::r, kSecondaryModifier);
  init_shortcut<AddColumnEvent>(MenuAction::AddColumn,
                                cen::scancodes::c,
                                kSecondaryModifier);

  init_shortcut<RemoveRowEvent>(MenuAction::RemoveRow,
                                cen::scancodes::r,
                                kSecondaryModifierAndShift);
  init_shortcut<RemoveColumnEvent>(MenuAction::RemoveColumn,
                                   cen::scancodes::c,
                                   kSecondaryModifierAndShift);
}

}  // namespace

void init_default_shortcuts()
{
  init_default_file_shortcuts();
  init_default_edit_shortcuts();
  init_default_view_shortcuts();
  init_default_map_shortcuts();
}

void update_shortcuts(const cen::keyboard_event& event, entt::dispatcher& dispatcher)
{
  for (const auto& shortcut: gShortcuts) {
    if (get_menu_item(shortcut.action).enabled &&  //
        event.pressed() &&                         //
        event.scan() == shortcut.key &&            //
        event.is_only_active(shortcut.modifiers)) {
      shortcut.activate(dispatcher);
    }
  }
}

}  // namespace tactile
