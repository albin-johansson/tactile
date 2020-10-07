#pragma once

#include <qstring.h>

#include "map_document.hpp"
#include "tileset_manager.hpp"

namespace tactile::service {

struct open_map_result final
{
  core::map_document* document{};
  core::tileset_manager tilesets;
};

[[nodiscard]] auto open_map(const QString& path) -> open_map_result;

}  // namespace tactile::service
