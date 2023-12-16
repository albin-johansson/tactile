// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <pugixml.hpp>

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_object_node(pugi::xml_node object_node)
    -> Result<ir::Object>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_objects(pugi::xml_node parent_node)
    -> Result<Vector<ir::Object>>;

}  // namespace tactile::tiled::tmx
