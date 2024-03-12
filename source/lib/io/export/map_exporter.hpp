// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(MapDocument)

/// Saves a map document to disk, inferring the destination from the document path.
/// The file format is also deduced from the document path.
auto save_map_document_to_disk(const MapDocument& document) -> Result<void>;

}  // namespace tactile
