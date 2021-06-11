#pragma once

#include <QString>  // QString

#include "map_document.hpp"

namespace tactile {

void SaveMapDocumentAsXml(const QString& path,
                          const core::MapDocument& document);

}  // namespace tactile
