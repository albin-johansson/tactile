#pragma once

#include <tactile_io.hpp>

#include "editor/document.hpp"

namespace Tactile {

class TextureManager;

/**
 * \brief Creates a map document from an intermediate map representation.
 *
 * \ingroup io
 *
 * \param data the intermediate representation of the map data.
 * \param textures the texture manager that will be used.
 *
 * \return a map document based on the intermediate representation.
 */
[[nodiscard]] auto CreateDocumentFromIR(const IO::Map& data, TextureManager& textures)
    -> Document;

}  // namespace Tactile
