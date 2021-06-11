#pragma once

#include "map_document.hpp"
#include "map_parse_data.hpp"

namespace tactile::parse {

[[nodiscard]] auto to_map_document(const map_data& data) -> core::MapDocument*;

}
