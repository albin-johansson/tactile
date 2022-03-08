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

class document_model;
class texture_manager;

/// \addtogroup io
/// \{

/**
 * \brief Restores the maps from the previous session.
 *
 * \param model the model that the previous session will be restored to.
 * \param textures the texture manager that will be used.
 *
 * \see `save_session()`
 */
void restore_last_session(document_model& model, texture_manager& textures);

/**
 * \brief Saves the current session.
 *
 * \param model the model that represent the current session that will be saved.
 *
 * \see `restore_last_session()`
 */
void save_session(const document_model& model);

/// \} End of group io

}  // namespace tactile
