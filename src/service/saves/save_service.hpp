#pragma once

#include <qurl.h>

#include "map_document.hpp"

namespace tactile::service {

void save(const QString& path, const core::map_document& document);

}
