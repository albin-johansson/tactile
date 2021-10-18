#include "save_map_document_as_xml.hpp"

#include <filesystem>  // path, relative, absolute

#include <pugixml.hpp>  // xml_document, xml_node

#include "append_layer.hpp"
#include "append_properties.hpp"
#include "append_tileset.hpp"
#include "save_xml.hpp"
#include "tactile/core/components/layer.hpp"
#include "tactile/core/components/layer_tree_node.hpp"
#include "tactile/core/components/parent.hpp"
#include "tactile/core/components/tileset.hpp"
#include "tactile/core/map.hpp"
#include "tactile/io/saving/common_saving.hpp"

namespace Tactile::IO {

void AppendMapChild(const entt::registry& registry,
                    pugi::xml_document& xml,
                    const std::filesystem::path& dir)
{
  auto node = xml.append_child("map");
  node.append_attribute("version").set_value(tiled_xml_version);
  node.append_attribute("tiledversion").set_value(tiled_version);
  node.append_attribute("orientation").set_value("orthogonal");
  node.append_attribute("renderorder").set_value("right-down");
  node.append_attribute("infinite").set_value(0);

  const auto& map = registry.ctx<Map>();
  node.append_attribute("tilewidth").set_value(map.tile_width);
  node.append_attribute("tileheight").set_value(map.tile_height);
  node.append_attribute("width").set_value(map.column_count);
  node.append_attribute("height").set_value(map.row_count);
  node.append_attribute("nextlayerid").set_value(map.next_layer_id.get());
  node.append_attribute("nextobjectid").set_value(map.next_object_id.get());

  AppendProperties(registry, entt::null, node, dir);

  for (auto&& [entity, tileset] : registry.view<Tileset>().each()) {
    AppendTileset(node, registry, entity, dir);
  }

  for (auto&& [entity, treeNode] : registry.view<LayerTreeNode>().each()) {
    const auto& parent = registry.get<Parent>(entity);
    if (parent.entity == entt::null) {
      AppendLayer(node, registry, entity, dir);
    }
  }
}

void SaveMapDocumentAsXml(const Document& document)
{
  const auto path = std::filesystem::absolute(document.path);
  const auto dir = path.parent_path();

  pugi::xml_document xml;
  AppendMapChild(document.registry, xml, dir);

  SaveXml(xml, path);
}

}  // namespace Tactile::IO
