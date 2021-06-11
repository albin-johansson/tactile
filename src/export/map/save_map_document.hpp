#pragma once

#include <QString>  // QString

#include "map_document.hpp"

namespace tactile {

void SaveMapDocument(const QString& path, const core::MapDocument& document);

}  // namespace tactile
