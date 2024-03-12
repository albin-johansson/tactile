// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "io/map/parse/parse_error.hpp"
#include "io/map/parse/parse_result.hpp"
#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/vector.hpp"

namespace tactile {

[[nodiscard]] auto parse_yaml_map(const Path& path) -> ParseResult;

[[nodiscard]] auto parse_tilesets(const YAML::Node& sequence,
                                  const MapIR& map,
                                  const Path& dir)
    -> Expected<Vector<TilesetIR>, ParseError>;

[[nodiscard]] auto parse_layers(const YAML::Node& sequence, const MapIR& map)
    -> Expected<Vector<LayerIR>, ParseError>;

[[nodiscard]] auto parse_object(const YAML::Node& node, const MapIR& map)
    -> Expected<ObjectIR, ParseError>;

[[nodiscard]] auto parse_component_definitions(const YAML::Node& node)
    -> Expected<ComponentMap, ParseError>;

[[nodiscard]] auto parse_properties(const YAML::Node& node)
    -> Expected<AttributeMap, ParseError>;

[[nodiscard]] auto parse_components(const YAML::Node& node, const MapIR& map)
    -> Expected<ComponentMap, ParseError>;

[[nodiscard]] auto parse_context(const YAML::Node& node, const MapIR& map)
    -> Expected<ContextIR, ParseError>;

}  // namespace tactile
