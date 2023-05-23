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

#include "common/enum/lang.hpp"
#include "common/enum/overlay_pos.hpp"
#include "common/enum/theme.hpp"
#include "model/settings.hpp"

namespace tactile {

/// Event for opening the settings dialog.
struct ShowSettingsEvent final {};

/// Event for overwriting the current settings.
struct SetSettingsEvent final {
  Settings settings;  ///< The new settings.
};

/// Event for setting the value of a single flag setting.
struct SetFlagSettingEvent final {
  SettingsFlagBits flag {};  ///< The setting to modify.
  bool value {};             ///< The new value.
};

/// Event for negating the value of a single flag setting.
struct NegateFlagSettingEvent final {
  SettingsFlagBits flag {};  ///< The setting to negate.
};

/// Event for setting the viewport overlay position.
struct SetViewportOverlayPosEvent final {
  OverlayPos pos {};  ///< The new viewport overlay position.
};

/// Event for setting the current display language.
struct SetLanguageEvent final {
  Lang language {};  ///< The new display language.
};

/// Event for setting the editor theme.
struct SetThemeEvent final {
  Theme theme {};  ///< The new editor theme.
};

/// Event for resetting the visibility of all dock widgets.
struct ResetDockVisibilitiesEvent final {};

}  // namespace tactile
