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

#include <entt/entt.hpp>

#include "editor/gui/components/component_editor.hpp"
#include "editor/gui/dialogs/settings_dialog.hpp"

namespace tactile {

class document_model;

/// \addtogroup gui
/// \{

class EditMenu final
{
 public:
  void Update(const document_model& model, entt::dispatcher& dispatcher);

  /* Updates all associated windows. */
  void UpdateWindows(const document_model& model, entt::dispatcher& dispatcher);

  void OpenSettingsModal();

  void ShowComponentEditor(const document_model& model);

 private:
  settings_dialog mSettingsDialog;
  component_editor mComponentEditor;
};

/// \} End of group gui

}  // namespace tactile
