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

#pragma once

#include "editor/gui/dialogs/dialog.hpp"
#include "io/persistence/preferences.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Provides the interface for all available persistent settings.
 *
 * \ingroup gui
 */
class SettingsDialog final : public ADialog
{
 public:
  TACTILE_DEFAULT_COPY(SettingsDialog);
  TACTILE_DEFAULT_MOVE(SettingsDialog);

  SettingsDialog();

  ~SettingsDialog() override = default;

  void show();

 protected:
  void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_cancel() override;

  void on_accept(entt::dispatcher& dispatcher) override;

  void on_apply(entt::dispatcher& dispatcher) override;

 private:
  PreferenceState mSnapshot;     ///< The original settings when the dialog was opened.
  PreferenceState mGuiSettings;  ///< The value of the settings in the GUI.

  void apply_settings(entt::dispatcher& dispatcher);

  void update_behavior_tab();

  void update_appearance_tab();

  void update_export_tab();
};

}  // namespace tactile
