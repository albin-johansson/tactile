#pragma once

#include "editor/document.hpp"

namespace tactile {

/**
 * \brief Saves a map document.
 *
 * \param document the map document that will be saved.
 *
 * \ingroup io
 */
void save_document(const Document& document);

}  // namespace tactile
