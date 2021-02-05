#pragma once

#include <QString>  // QString

#include "map_document.hpp"

namespace tactile {

/**
 * \brief Saves a map using the Tiled JSON format.
 *
 * \param path the destination path of of the map file.
 * \param map the document that will be saved.
 *
 * \since 0.1.0
 */
void save_map_document_as_json(const QString& path,
                               const core::map_document& map);

}  // namespace tactile
