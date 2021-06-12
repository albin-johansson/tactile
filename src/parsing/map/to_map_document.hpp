#pragma once

#include "map_document.hpp"
#include "map_parse_data.hpp"

namespace tactile {

[[nodiscard]] auto ToMapDocument(const ir::MapData& data) -> core::MapDocument*;

}  // namespace tactile
