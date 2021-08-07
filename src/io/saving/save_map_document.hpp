#pragma once

#include "core/map_document.hpp"

namespace Tactile::IO {

/**
 * \brief Saves a map document.
 *
 * \ingroup io
 *
 * \param document the map document that will be saved.
 */
void SaveMapDocument(const MapDocument& document);

}  // namespace Tactile::IO
