#pragma once

#include <tactile-io/ir.hpp>

#include "editor/document.hpp"

namespace Tactile {

/**
 * \brief Creates a map document from an intermediate map representation.
 *
 * \ingroup io
 *
 * \param data the intermediate representation of the map data.
 *
 * \return a map document based on the intermediate representation.
 */
[[nodiscard]] auto CreateDocumentFromIR(const IO::Map& data) -> Document;

}  // namespace Tactile
