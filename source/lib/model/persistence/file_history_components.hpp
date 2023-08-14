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

#include "core/containers/deque.hpp"
#include "tactile/core/containers/string.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "core/prelude.hpp"

namespace tactile {

inline constexpr usize kFileHistoryMaxSize = 10;

/**
 * Context component tracking previously opened files.
 *
 * \details We store paths as strings because that makes displaying them in menus much
 * easier (and faster).
 */
struct FileHistory final {
  Maybe<String> last_closed_file;  ///< Path to the last closed document, if any.
  Deque<String> entries;           ///< Paths to previously opened files.
};

}  // namespace tactile
