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

#include "common/type/dispatcher.hpp"
#include "common/type/vec.hpp"
#include "model/event/all.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/shortcut/shortcut.hpp"

namespace tactile {
namespace {

inline Vec<Shortcut> gShortcuts;

template <typename Event>
void _init_shortcut(const MenuAction action,
                    const cen::scan_code key,
                    const cen::key_mod modifiers = cen::key_mod::none)
{
  gShortcuts.push_back(Shortcut {
      .action = action,
      .key = key,
      .modifiers = modifiers,
      .activate = [](Dispatcher& dispatcher) { dispatcher.enqueue<Event>(); },
  });
}

void _init_tool_shortcut(const ToolType tool,
                         const MenuAction action,
                         const cen::scan_code key)
{
  gShortcuts.push_back(Shortcut {.action = action,
                                 .key = key,
                                 .modifiers = cen::key_mod::none,
                                 .activate = [=](Dispatcher& dispatcher) {
                                   dispatcher.enqueue<SelectToolEvent>(tool);
                                 }});
}

void _init_default_file_shortcuts()
{
  _init_shortcut<ShowNewMapDialogEvent>(MenuAction::NewMap,
                                        cen::scancodes::n,
                                        kPrimaryModifier);
  _init_shortcut<ShowOpenMapDialogEvent>(MenuAction::OpenMap,
                                         cen::scancodes::o,
                                         kPrimaryModifier);
  _init_shortcut<SaveEvent>(MenuAction::Save, cen::scancodes::s, kPrimaryModifier);
  _init_shortcut<ShowSaveAsDialogEvent>(MenuAction::SaveAs,
                                        cen::scancodes::s,
                                        kPrimaryModifierAndShift);
}

void _init_default_edit_shortcuts()
{
  _init_shortcut<UndoEvent>(MenuAction::Undo, cen::scancodes::z, kPrimaryModifier);
  _init_shortcut<RedoEvent>(MenuAction::Redo, cen::scancodes::y, kPrimaryModifier);

  _init_tool_shortcut(ToolType::Stamp, MenuAction::EnableStamp, cen::scancodes::s);
  _init_tool_shortcut(ToolType::Eraser, MenuAction::EnableEraser, cen::scancodes::e);
  _init_tool_shortcut(ToolType::Bucket, MenuAction::EnableBucket, cen::scancodes::b);
  _init_tool_shortcut(ToolType::ObjectSelection,
                      MenuAction::EnableObjectSelector,
                      cen::scancodes::q);
  _init_tool_shortcut(ToolType::Rectangle,
                      MenuAction::EnableRectangle,
                      cen::scancodes::r);
  _init_tool_shortcut(ToolType::Ellipse, MenuAction::EnableEllipse, cen::scancodes::t);
  _init_tool_shortcut(ToolType::Point, MenuAction::EnablePoint, cen::scancodes::y);

  _init_shortcut<OpenComponentEditorEvent>(MenuAction::OpenComponentEditor,
                                           cen::scancodes::c,
                                           kPrimaryModifierAndShift);
  _init_shortcut<ShowSettingsEvent>(MenuAction::OpenSettings,
                                    SDL_SCANCODE_COMMA,
                                    kPrimaryModifier);
}

void _init_default_view_shortcuts()
{
  const cen::scan_code plus {SDLK_PLUS};
  const cen::scan_code minus {SDLK_MINUS};

  _init_shortcut<CenterViewportEvent>(MenuAction::CenterViewport,
                                      cen::scancodes::space,
                                      cen::key_mod::lshift);

  _init_shortcut<IncreaseZoomEvent>(MenuAction::IncreaseZoom, plus, kPrimaryModifier);
  _init_shortcut<DecreaseZoomEvent>(MenuAction::DecreaseZoom, minus, kPrimaryModifier);

  _init_shortcut<IncreaseFontSizeEvent>(MenuAction::IncreaseFontSize,
                                        plus,
                                        kPrimaryModifierAndShift);
  _init_shortcut<DecreaseFontSizeEvent>(MenuAction::DecreaseFontSize,
                                        minus,
                                        kPrimaryModifierAndShift);

  _init_shortcut<ToggleGridEvent>(MenuAction::ToggleGrid,
                                  cen::scancodes::g,
                                  kPrimaryModifier);
  _init_shortcut<ToggleHighlightLayerEvent>(MenuAction::HighlightLayer,
                                            cen::scancodes::h);
  _init_shortcut<ToggleUiEvent>(MenuAction::ToggleUi, cen::scancodes::tab);

  _init_shortcut<PanRightEvent>(MenuAction::PanRight,
                                cen::scancodes::right,
                                kPrimaryModifierAndShift);
  _init_shortcut<PanLeftEvent>(MenuAction::PanLeft,
                               cen::scancodes::left,
                               kPrimaryModifierAndShift);
  _init_shortcut<PanUpEvent>(MenuAction::PanUp,
                             cen::scancodes::up,
                             kPrimaryModifierAndShift);
  _init_shortcut<PanDownEvent>(MenuAction::PanDown,
                               cen::scancodes::down,
                               kPrimaryModifierAndShift);
}

void _init_default_map_shortcuts()
{
  _init_shortcut<ShowTilesetCreationDialogEvent>(MenuAction::CreateTileset,
                                                 cen::scancodes::t,
                                                 kPrimaryModifier);

  _init_shortcut<AddRowEvent>(MenuAction::AddRow, cen::scancodes::r, kSecondaryModifier);
  _init_shortcut<AddColumnEvent>(MenuAction::AddColumn,
                                 cen::scancodes::c,
                                 kSecondaryModifier);

  _init_shortcut<RemoveRowEvent>(MenuAction::RemoveRow,
                                 cen::scancodes::r,
                                 kSecondaryModifierAndShift);
  _init_shortcut<RemoveColumnEvent>(MenuAction::RemoveColumn,
                                    cen::scancodes::c,
                                    kSecondaryModifierAndShift);
}

}  // namespace

void init_default_shortcuts()
{
  _init_default_file_shortcuts();
  _init_default_edit_shortcuts();
  _init_default_view_shortcuts();
  _init_default_map_shortcuts();
}

void update_shortcuts(const cen::keyboard_event& event, entt::dispatcher& dispatcher)
{
  for (const auto& shortcut: gShortcuts) {
    // TODO
    //    if (get_menu_item(shortcut.action).enabled &&  //
    //        event.pressed() &&                         //
    //        event.scan() == shortcut.key &&            //
    //        event.is_only_active(shortcut.modifiers)) {
    //      shortcut.activate(dispatcher);
    //    }
  }
}

}  // namespace tactile
