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

#include "common/macros.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(DocumentModel)
TACTILE_FWD_DECLARE_CLASS(ParseResult)

/// Restores a map document from an intermediate map representation.
///
/// \param result the intermediate representation of the map data.
/// \param model the target document model.
void create_map_document_from_ir(const ParseResult& result, DocumentModel& model);

}  // namespace tactile
