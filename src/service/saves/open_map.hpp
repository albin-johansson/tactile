#pragma once

#include <qstring.h>

#include "map_document.hpp"

namespace tactile::service {

[[nodiscard]] auto open_map(const QString& path) -> core::map_document*;

}  // namespace tactile::service
