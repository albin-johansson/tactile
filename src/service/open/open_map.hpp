#pragma once

#include <QString>

#include "map_document.hpp"

namespace tactile::service {

/**
 * @brief Creates and returns a tilemap based on a save file.
 *
 * @warning You must claim ownership of the returned map document. The map
 * document has no parent, so you should set the parent as quickly as possible
 * to avoid a memory leak.
 *
 * @param path the path of the save file.
 *
 * @return a pointer to the created map document.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto open_map(const QString& path) -> core::map_document*;

}  // namespace tactile::service
