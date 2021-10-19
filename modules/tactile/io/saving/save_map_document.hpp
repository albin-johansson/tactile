#pragma once

#include "editor/document.hpp"

namespace Tactile::IO {

/**
 * \brief Saves a map document.
 *
 * \ingroup io
 *
 * \param document the map document that will be saved.
 */
void SaveMapDocument(const Document& document);

}  // namespace Tactile::IO
