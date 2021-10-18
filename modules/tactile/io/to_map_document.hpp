#pragma once

#include "tactile/editor/document.hpp"

namespace Tactile::IO {

struct MapData;

/**
 * \brief Creates a map document from an intermediate map representation.
 *
 * \ingroup io
 *
 * \param data the intermediate representation of the map data.
 *
 * \return a map document based on the intermediate representation.
 */
[[nodiscard]] auto ToMapDocument(const MapData& data) -> Document;

}  // namespace Tactile::IO
