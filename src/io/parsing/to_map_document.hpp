#pragma once

#include "aliases/unique.hpp"
#include "core/map_document.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ToMapDocument(const MapData& data) -> Unique<MapDocument>;

}  // namespace Tactile::IO
