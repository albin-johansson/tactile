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
#include "model/events/setting_events.hpp"
#include "model/registry.hpp"

namespace tactile {

void on_show_settings(Registry& registry, const ShowSettingsEvent& event);

void on_set_settings(Registry& registry,
                     Dispatcher& dispatcher,
                     const SetSettingsEvent& event);

void on_set_flag_setting(Registry& registry, const SetFlagSettingEvent& event);

void on_negate_flag_setting(Registry& registry, const NegateFlagSettingEvent& event);

void on_set_viewport_overlay_pos(Registry& registry,
                                 const SetViewportOverlayPosEvent& event);

void on_set_language(Registry& registry,
                     Dispatcher& dispatcher,
                     const SetLanguageEvent& event);

void on_set_theme(Registry& registry, const SetThemeEvent& event);

void on_reset_dock_visibilities(Registry& registry,
                                const ResetDockVisibilitiesEvent& event);

}  // namespace tactile
