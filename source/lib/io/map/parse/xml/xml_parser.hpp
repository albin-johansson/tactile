// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/parse_error.hpp"
#include "io/map/parse/parse_result.hpp"
#include "io/util/xml.hpp"
#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"

namespace tactile {

[[nodiscard]] auto parse_xml_map(const Path& path) -> ParseResult;

[[nodiscard]] auto parse_tileset(XmlNode node, const Path& dir)
    -> Expected<TilesetIR, ParseError>;

[[nodiscard]] auto parse_object(XmlNode object_node) -> Expected<ObjectIR, ParseError>;

[[nodiscard]] auto parse_layers(XmlNode map_node, MapIR& map)
    -> Expected<Vector<LayerIR>, ParseError>;

[[nodiscard]] auto parse_properties(XmlNode node) -> Expected<AttributeMap, ParseError>;

[[nodiscard]] auto parse_context(XmlNode node) -> Expected<ContextIR, ParseError>;

}  // namespace tactile
