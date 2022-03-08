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

#include "editor/gui/dialogs/about_dialog.hpp"
#include "editor/gui/dialogs/credits_dialog.hpp"

namespace tactile {

class document_model;

class HelpMenu final
{
 public:
  void Update(const document_model& model, entt::dispatcher& dispatcher);

  void UpdateWindows();

 private:
  about_dialog mAboutDialog;
  credits_dialog mCreditsDialog;
  bool mShowAboutImGui{};
};

}  // namespace tactile
