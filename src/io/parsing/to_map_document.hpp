#pragma once

#include "aliases/unique.hpp"
#include "core/map_document.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

/**
 * \brief Creates a map document from an intermediate map representation.
 *
 * \ingroup io
 *
 * \param data the intermediate representation of the map data.
 *
 * \return a map document based on the intermediate representation.
 */
[[nodiscard]] auto ToMapDocument(const MapData& data) -> Unique<MapDocument>;

}  // namespace Tactile::IO
