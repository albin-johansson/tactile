#pragma once

#include <tactile_io.hpp>

#include "editor/document.hpp"

namespace Tactile {

class TextureManager;

/**
 * \brief Creates a map document from an intermediate map representation.
 *
 * \param irMap the intermediate representation of the map data.
 * \param textures the texture manager that will be used.
 *
 * \return a document based on the intermediate representation.
 */
[[nodiscard]] auto CreateDocumentFromIR(const IO::Map& irMap, TextureManager& textures)
    -> Document;

}  // namespace Tactile
