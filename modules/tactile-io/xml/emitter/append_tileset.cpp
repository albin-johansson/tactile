#include "append_tileset.hpp"

#include <format>  // format

#include "../../common_saving.hpp"
#include "append_fancy_tiles.hpp"
#include "append_properties.hpp"
#include "emitter.hpp"
#include "save_xml.hpp"

namespace Tactile::IO {
namespace {

void AddCommon(pugi::xml_node node,
               const TilesetData& tileset,
               const std::filesystem::path& dir)
{
  node.append_attribute("name").set_value(tileset.name.c_str());
  node.append_attribute("tilewidth").set_value(tileset.tile_width);
  node.append_attribute("tileheight").set_value(tileset.tile_height);
  node.append_attribute("tilecount").set_value(tileset.tile_count);
  node.append_attribute("columns").set_value(tileset.column_count);

  {
    auto imageNode = node.append_child("image");
    const auto source = GetTilesetImagePath(tileset.absolute_image_path, dir);
    imageNode.append_attribute("source").set_value(source.c_str());
    imageNode.append_attribute("width").set_value(tileset.image_width);
    imageNode.append_attribute("height").set_value(tileset.image_height);
  }

  AppendFancyTiles(node, tileset.tiles, dir);
  AppendProperties(node, tileset.properties, dir);
}

void AppendEmbeddedTileset(pugi::xml_node mapNode,
                           const TilesetData& tileset,
                           const std::filesystem::path& dir)
{
  auto node = mapNode.append_child("tileset");
  node.append_attribute("firstgid").set_value(tileset.first_id);

  AddCommon(node, tileset, dir);
}

void CreateExternalTilesetFile(const TilesetData& tileset,
                               const std::string& source,
                               const std::filesystem::path& dir)
{
  pugi::xml_document xml;

  auto root = xml.append_child("tileset");
  root.append_attribute("version").set_value(tiled_xml_version);
  root.append_attribute("tiledversion").set_value(tiled_version);

  AddCommon(root, tileset, dir);

  const auto path = dir / source;
  CENTURION_LOG_INFO("Saving external tileset in \"%s\"", path.string().c_str());

  SaveXml(xml, path);
}

void AppendExternalTileset(pugi::xml_node mapNode,
                           const TilesetData& tileset,
                           const std::string& source)
{
  auto node = mapNode.append_child("tileset");
  node.append_attribute("firstgid").set_value(tileset.first_id);
  node.append_attribute("source").set_value(source.c_str());
}

}  // namespace

void AppendTileset(pugi::xml_node mapNode,
                   const TilesetData& tileset,
                   const std::filesystem::path& dir,
                   const EmitterOptions& options)
{
  if (options.embed_tilesets) {
    AppendEmbeddedTileset(mapNode, tileset, dir);
  }
  else {
    const auto source = std::format("{}.tsx", tileset.name);
    CreateExternalTilesetFile(tileset, source, dir);
    AppendExternalTileset(mapNode, tileset, source);
  }
}

}  // namespace Tactile::IO
