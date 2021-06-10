#pragma once

#include <QString>  // QString

#include "map_document.hpp"

namespace tactile {

void save_map_document(const QString& path, const core::MapDocument& document);

}
