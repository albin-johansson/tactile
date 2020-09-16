#pragma once

#include <qstring.h>

#include "map.hpp"
#include "tileset_manager.hpp"

namespace tactile::service {

void save_tmx(const QString& path,
              const core::map& map,
              const core::tileset_manager& tilesets);

}  // namespace tactile::service
