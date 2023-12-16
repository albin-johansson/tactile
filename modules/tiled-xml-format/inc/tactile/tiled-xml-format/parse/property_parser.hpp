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
TACTILE_TMX_FORMAT_API auto parse_property_node(pugi::xml_node property_node)
    -> Result<ir::NamedAttribute>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_properties_node(pugi::xml_node context_node)
    -> Result<Vector<ir::NamedAttribute>>;

}  // namespace tactile::tiled::tmx
