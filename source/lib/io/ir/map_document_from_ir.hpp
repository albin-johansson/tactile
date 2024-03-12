// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(DocumentModel)
TACTILE_FWD_DECLARE_STRUCT(MapIR)

/// Restores a map document from an intermediate map representation.
///
/// \param ir_map the intermediate representation of the map data.
/// \param document_path the path to the document.
/// \param model the target document model.
void create_map_document_from_ir(const MapIR& ir_map,
                                 const Path& document_path,
                                 DocumentModel& model);

}  // namespace tactile
