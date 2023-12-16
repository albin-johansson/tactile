// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/parse/meta_parser.hpp"

#include "tactile/tiled-xml-format/parse/property_parser.hpp"

namespace tactile::tiled::tmx {

auto parse_metadata(pugi::xml_node context_node, ir::Metadata& metadata) -> Result<void>
{
  return parse_properties_node(context_node)
      .and_then([&](Vector<ir::NamedAttribute>&& properties) {
        metadata.properties = std::move(properties);
        return kOK;
      });
}

}  // namespace tactile::tiled::tmx
