#pragma once

#include "editor/document.hpp"
#include "parser/parse_data.hpp"

namespace tactile {

class texture_manager;

/**
 * \brief Creates a map document from an intermediate map representation.
 *
 * \param data the intermediate representation of the map data.
 * \param textures the texture manager that will be used.
 *
 * \return a document based on the intermediate representation.
 */
[[nodiscard]] auto restore_document_from_ir(const parsing::parse_data& data,
                                            texture_manager& textures) -> Document;

}  // namespace tactile
