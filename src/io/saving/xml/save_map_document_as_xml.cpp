#include "save_map_document_as_xml.hpp"

#include <filesystem>   // path, relative
#include <pugixml.hpp>  // xml_document, xml_node

#include "io/saving/common.hpp"
#include "io/saving/xml/append_layer.hpp"
#include "io/saving/xml/append_properties.hpp"
#include "io/saving/xml/append_tileset.hpp"
#include "io/saving/xml/save_xml.hpp"

namespace Tactile::IO {

void AppendMapChild(const MapDocument& document,
                    pugi::xml_document& xml,
                    const std::filesystem::path& dir)
{
  auto node = xml.append_child("map");
  node.append_attribute("version").set_value(tiled_xml_version);
  node.append_attribute("tiledversion").set_value(tiled_version);
  node.append_attribute("orientation").set_value("orthogonal");
  node.append_attribute("renderorder").set_value("right-down");
  node.append_attribute("infinite").set_value(0);

  const auto& map = document.GetMap();
  node.append_attribute("tilewidth").set_value(32);   // FIXME
  node.append_attribute("tileheight").set_value(32);  // FIXME
  node.append_attribute("width").set_value(map.GetColumnCount().get());
  node.append_attribute("height").set_value(map.GetRowCount().get());
  node.append_attribute("nextlayerid").set_value(map.GetNextLayerId().get());
  node.append_attribute("nextobjectid").set_value(map.GetNextObjectId().get());

  AppendProperties(document, node, dir);

  for (const auto& [id, tileset] : document.GetTilesets())
  {
    AppendTileset(*tileset, node, dir);
  }

  for (const auto& [id, layer] : map)
  {
    AppendLayer(id, *layer, node, dir);
  }
}

void SaveMapDocumentAsXml(const MapDocument& document)
{
  const auto path = document.GetAbsolutePath();
  const auto dir = path.parent_path();

  pugi::xml_document xml;
  AppendMapChild(document, xml, dir);

  SaveXml(xml, path);
}

}  // namespace Tactile::IO
