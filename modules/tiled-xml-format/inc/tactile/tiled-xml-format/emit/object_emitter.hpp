// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <pugixml.hpp>

#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

TACTILE_TMX_FORMAT_API void append_object_node(pugi::xml_node parent_node,
                                               const ir::Object& object);

}  // namespace tactile::tiled::tmx
