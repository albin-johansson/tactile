// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/parse/meta_parser.hpp"

#include <utility>  // move

#include "tactile/foundation/log/logger.hpp"
#include "tactile/tactile-yml-format/parse/common.hpp"
#include "tactile/tactile-yml-format/parse/component_parser.hpp"
#include "tactile/tactile-yml-format/parse/property_parser.hpp"

namespace tactile::yml_format {

auto parse_metadata(const YAML::Node& context_node,
                    const ir::Map& map,
                    ir::Metadata& metadata) -> Result<void>
{
  TACTILE_LOG_TRACE("Parsing metadata from node at {}:{}...",
                    context_node.Mark().line,
                    context_node.Mark().column);

  return parse_attached_components(context_node, map)
      .and_then([&](Vector<ir::AttachedComponent>&& components) {
        metadata.components = std::move(components);
        return kOK;
      })
      .and_then([&] { return parse_properties(context_node); })
      .and_then([&](Vector<ir::NamedAttribute>&& properties) {
        metadata.properties = std::move(properties);
        return kOK;
      })
      .and_then([&] {
        (void) try_parse_to(context_node, "name", metadata.name);
        return kOK;
      });
}

}  // namespace tactile::yml_format
