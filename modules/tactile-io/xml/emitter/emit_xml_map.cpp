#include <filesystem>  // path, relative, absolute

#include <pugixml.hpp>  // xml_document, xml_node

#include "../../common_saving.hpp"
#include "append_layer.hpp"
#include "append_properties.hpp"
#include "append_tileset.hpp"
#include "emitter.hpp"
#include "save_xml.hpp"

namespace Tactile::IO {

void AppendMapChild(pugi::xml_document& xml,
                    const MapData& map,
                    const std::filesystem::path& dir,
                    const EmitterOptions& options)
{
  auto node = xml.append_child("map");
  node.append_attribute("version").set_value(tiled_xml_version);
  node.append_attribute("tiledversion").set_value(tiled_version);
  node.append_attribute("orientation").set_value("orthogonal");
  node.append_attribute("renderorder").set_value("right-down");
  node.append_attribute("infinite").set_value(0);

  node.append_attribute("tilewidth").set_value(map.tile_width);
  node.append_attribute("tileheight").set_value(map.tile_height);
  node.append_attribute("width").set_value(map.column_count);
  node.append_attribute("height").set_value(map.row_count);
  node.append_attribute("nextlayerid").set_value(map.next_layer_id);
  node.append_attribute("nextobjectid").set_value(map.next_object_id);

  AppendProperties(node, map.properties, dir);

  for (const auto& tileset : map.tilesets) {
    AppendTileset(node, tileset, dir, options);
  }

  for (const auto& layer : map.layers) {
    AppendLayer(node, layer, dir, options.human_readable_output);
  }
}

void EmitXmlMap(const MapData& data, const EmitterOptions& options)
{
  pugi::xml_document xml;
  AppendMapChild(xml, data, data.absolute_path.parent_path(), options);
  SaveXml(xml, data.absolute_path);
}

}  // namespace Tactile::IO
