#pragma once

#include <tactile_io.hpp>

namespace tactile {

struct Document;

/**
 * \brief Converts a document to the intermediate representation used by the Tactile IO
 * library.
 *
 * \param document the document that will be converted.
 *
 * \return a unique pointer to the intermediate representation.
 */
[[nodiscard]] auto ConvertDocumentToIR(const Document& document) -> IO::MapPtr;

}  // namespace tactile