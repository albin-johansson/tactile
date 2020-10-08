#pragma once

#include <qstring.h>

#include "map_document.hpp"

namespace tactile::service {

/**
 * @brief Saves a map using the Tiled JSON format.
 *
 * @param path the destination path of of the map file.
 * @param map the document that will be saved.
 *
 * @since 0.1.0
 */
void save_json(const QString& path, const core::map_document& map);

}  // namespace tactile::service
