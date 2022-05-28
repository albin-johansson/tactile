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

namespace tactile {

/**
 * \brief Shows a checkbox.
 *
 * \ingroup gui
 *
 * \param label the checkbox label.
 * \param[out] value a pointer to which the value of the checkbox will be written.
 * \param tooltip optional tooltip text.
 *
 * \return `true` if the value changed; `false` otherwise.
 */
auto checkbox(const char* label, bool* value, const char* tooltip = nullptr) -> bool;

}  // namespace tactile