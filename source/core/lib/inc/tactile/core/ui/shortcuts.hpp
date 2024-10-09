// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/base/prelude.hpp"

#if TACTILE_OS_APPLE
  #define TACTILE_SHORTCUT "Cmd"
#else
  #define TACTILE_SHORTCUT "Ctrl"
#endif

namespace tactile::core {

class Model;
class EventDispatcher;

namespace ui {

/**
 * Provides information about an editor shortcut.
 */
struct ShortcutInfo final
{
  /** The associated key combination. */
  ImGuiKeyChord chord;

  /** The key combination hint (for use in menus). */
  const char* hint;
};

inline constexpr ShortcutInfo kCreateMapShortcut {
  ImGuiMod_Shortcut | ImGuiKey_N,
  TACTILE_SHORTCUT " + N",
};

inline constexpr ShortcutInfo kOpenShortcut {
  ImGuiMod_Shortcut | ImGuiKey_O,
  TACTILE_SHORTCUT " + O",
};

inline constexpr ShortcutInfo kSaveShortcut {
  ImGuiMod_Shortcut | ImGuiKey_S,
  TACTILE_SHORTCUT " + S",
};

inline constexpr ShortcutInfo kSaveAsShortcut {
  ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_S,
  TACTILE_SHORTCUT " + Shift + S",
};

inline constexpr ShortcutInfo kOpenSettingsShortcut {
  ImGuiMod_Shortcut | ImGuiKey_Comma,
  TACTILE_SHORTCUT " + ,",
};

inline constexpr ShortcutInfo kUndoShortcut {
  ImGuiMod_Shortcut | ImGuiKey_Z,
  TACTILE_SHORTCUT " + Z",
};

inline constexpr ShortcutInfo kRedoShortcut {
  ImGuiMod_Shortcut | ImGuiKey_Y,
  TACTILE_SHORTCUT " + Y",
};

inline constexpr ShortcutInfo kOpenComponentEditorShortcut {
  ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_C,
  TACTILE_SHORTCUT " + Shift + C",
};

inline constexpr ShortcutInfo kIncreaseFontSizeShortcut {
  ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_0,
  TACTILE_SHORTCUT " + Shift + 0",
};

inline constexpr ShortcutInfo kDecreaseFontSizeShortcut {
  ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_9,
  TACTILE_SHORTCUT " + Shift + 9",
};

inline constexpr ShortcutInfo kIncreaseZoomShortcut {
  ImGuiMod_Shortcut | ImGuiKey_0,
  TACTILE_SHORTCUT " + 0",
};

inline constexpr ShortcutInfo kDecreaseZoomShortcut {
  ImGuiMod_Shortcut | ImGuiKey_9,
  TACTILE_SHORTCUT " + 9",
};

inline constexpr ShortcutInfo kCenterViewportShortcut {
  ImGuiMod_Shift | ImGuiKey_Space,
  "Shift + Space",
};

inline constexpr ShortcutInfo kToggleGridShortcut {
  ImGuiMod_Shortcut | ImGuiKey_G,
  TACTILE_SHORTCUT " + G",
};

inline constexpr ShortcutInfo kPanLeftShortcut {
  ImGuiMod_Shortcut | ImGuiKey_LeftArrow,
  TACTILE_SHORTCUT " + Left",
};

inline constexpr ShortcutInfo kPanRightShortcut {
  ImGuiMod_Shortcut | ImGuiKey_RightArrow,
  TACTILE_SHORTCUT " + Right",
};

inline constexpr ShortcutInfo kPanUpShortcut {
  ImGuiMod_Shortcut | ImGuiKey_UpArrow,
  TACTILE_SHORTCUT " + Up",
};

inline constexpr ShortcutInfo kPanDownShortcut {
  ImGuiMod_Shortcut | ImGuiKey_DownArrow,
  TACTILE_SHORTCUT " + Down",
};

/**
 * Adds the global shortcut routes to the widget stack.
 *
 * \param model      The associated model.
 * \param dispatcher The event dispatcher to use.
 */
void push_global_shortcuts(const Model& model, EventDispatcher& dispatcher);

}  // namespace ui
}  // namespace tactile::core
