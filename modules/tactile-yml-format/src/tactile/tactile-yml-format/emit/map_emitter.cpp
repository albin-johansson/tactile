// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/emit/map_emitter.hpp"

#include "tactile/tactile-yml-format/emit/component_emitter.hpp"
#include "tactile/tactile-yml-format/emit/layer_emitter.hpp"
#include "tactile/tactile-yml-format/emit/meta_emitter.hpp"
#include "tactile/tactile-yml-format/emit/tile_format_emitter.hpp"
#include "tactile/tactile-yml-format/emit/tileset_emitter.hpp"

namespace tactile::yml_format {

void emit_map(YAML::Emitter& emitter,
              const ir::Map& map,
              const SaveFormatWriteOptions& options)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "version" << YAML::Value << 1;

  emitter << YAML::Key << "row-count" << YAML::Value << map.row_count;
  emitter << YAML::Key << "column-count" << YAML::Value << map.col_count;

  emitter << YAML::Key << "tile-width" << YAML::Value << map.tile_width;
  emitter << YAML::Key << "tile-height" << YAML::Value << map.tile_height;

  emitter << YAML::Key << "next-layer-id" << YAML::Value << map.next_layer_id;
  emitter << YAML::Key << "next-object-id" << YAML::Value << map.next_object_id;

  emit_tile_format(emitter, map.tile_format);
  emit_component_seq(emitter, map.components);
  emit_tileset_seq(emitter, map, options);
  emit_layer_seq(emitter, map, options);
  emit_metadata(emitter, map.meta);

  emitter << YAML::EndMap;
}

}  // namespace tactile::yml_format
