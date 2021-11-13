#include <filesystem>  // path, relative, absolute

#include <pugixml.hpp>  // xml_document, xml_node

#include "../../common_saving.hpp"
#include "append_layer.hpp"
#include "append_properties.hpp"
#include "append_tileset.hpp"
#include "emit.hpp"
#include "ir_helpers.hpp"
#include "save_xml.hpp"

namespace Tactile::IO {

void AppendMapChild(pugi::xml_document& xml,
                    const Map& map,
                    const std::filesystem::path& dir,
                    const EmitterOptions options)
{
  auto node = xml.append_child("map");
  node.append_attribute("version").set_value(tiled_xml_version);
  node.append_attribute("tiledversion").set_value(tiled_version);
  node.append_attribute("orientation").set_value("orthogonal");
  node.append_attribute("renderorder").set_value("right-down");
  node.append_attribute("infinite").set_value(0);

  node.append_attribute("tilewidth").set_value(GetTileWidth(map));
  node.append_attribute("tileheight").set_value(GetTileHeight(map));
  node.append_attribute("width").set_value(GetColumnCount(map));
  node.append_attribute("height").set_value(GetRowCount(map));
  node.append_attribute("nextlayerid").set_value(GetNextLayerId(map));
  node.append_attribute("nextobjectid").set_value(GetNextObjectId(map));

  AppendProperties(node, map, dir);

  EachTileset(map, [&](const Tileset& tileset) {
    AppendTileset(node, tileset, dir, options);
  });

  EachLayer(map, [&](const Layer& layer) {
    AppendLayer(node, layer, dir, options & EmitterOption_FoldTileData);
  });
}

void EmitXmlMap(const Map& data, const EmitterOptions options)
{
  pugi::xml_document xml;

  const std::filesystem::path path = GetPath(data);
  const auto dir = path.parent_path();

  AppendMapChild(xml, data, dir, options);
  SaveXml(xml, path);
}

}  // namespace Tactile::IO
