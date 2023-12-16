// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <pugixml.hpp>

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/meta/attribute_type.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto to_tmx_name(AttributeType type) -> const char*;

TACTILE_TMX_FORMAT_API void append_property_node(pugi::xml_node properties_node,
                                                 const ir::NamedAttribute& property);

TACTILE_TMX_FORMAT_API void append_properties_node(
    pugi::xml_node context_node,
    const Vector<ir::NamedAttribute>& properties);

}  // namespace tactile::tiled::tmx
