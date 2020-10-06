#pragma once

#include <qstring.h>

#include "map.hpp"
#include "tileset_manager.hpp"

namespace tactile::service {

/**
 * @brief Saves a map using the Tiled JSON format.
 *
 * @param path the destination path of of the map file.
 * @param map the map that will be saved.
 * @param tilesets the tilesets that are being used.
 *
 * @since 0.1.0
 */
void save_json(const QString& path,
               const core::map& map,
               const core::tileset_manager& tilesets);

}  // namespace tactile::service
