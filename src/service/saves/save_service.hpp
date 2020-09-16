#pragma once

#include <qurl.h>

#include "map.hpp"
#include "tileset_manager.hpp"

namespace tactile::service {

void save(const QUrl& url,
          const core::map& map,
          const core::tileset_manager& tilesets);

}
