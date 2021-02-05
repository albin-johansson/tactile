#pragma once

#include <QString>  // QString

#include "map_document.hpp"

namespace tactile {

void save_map_document_as_xml(const QString& path,
                              const core::map_document& document);

}  // namespace tactile
