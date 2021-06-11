#pragma once

#include "map_document.hpp"
#include "map_parse_data.hpp"

namespace tactile::parse {

[[nodiscard]] auto ToMapDocument(const MapData& data) -> core::MapDocument*;

}  // namespace tactile::parse
