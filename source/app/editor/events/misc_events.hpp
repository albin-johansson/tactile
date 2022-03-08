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

#include <filesystem>  // path

namespace tactile {

/// \addtogroup events
/// \{

struct quit_event final
{};

struct toggle_ui_event final
{};

struct show_settings_event final
{};

struct save_event final
{};

struct save_as_event final
{
  std::filesystem::path path;
};

struct open_save_as_dialog_event final
{};

/// \} End of group events

}  // namespace tactile