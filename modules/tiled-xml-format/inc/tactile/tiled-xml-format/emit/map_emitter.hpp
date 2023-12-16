// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <pugixml.hpp>

#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

TACTILE_TMX_FORMAT_API void append_map_node(pugi::xml_node root_node,
                                            const ir::Map& map,
                                            const SaveFormatWriteOptions& options);

}  // namespace tactile::tiled::tmx
