#include "append_tileset.hpp"

#include <fmt/format.h>  // format

#include "../../common_saving.hpp"
#include "append_fancy_tiles.hpp"
#include "append_properties.hpp"
#include "emit.hpp"
#include "save_xml.hpp"

namespace tactile::IO {
namespace {

void AddCommon(pugi::xml_node node,
               const Tileset& tileset,
               const std::filesystem::path& dir)
{
  node.append_attribute("name").set_value(GetName(tileset));
  node.append_attribute("tilewidth").set_value(GetTileWidth(tileset));
  node.append_attribute("tileheight").set_value(GetTileHeight(tileset));
  node.append_attribute("tilecount").set_value(GetTileCount(tileset));
  node.append_attribute("columns").set_value(GetColumnCount(tileset));

  {
    auto imageNode = node.append_child("image");
    const auto source = GetTilesetImagePath(GetImagePath(tileset), dir);
    imageNode.append_attribute("source").set_value(source.c_str());
    imageNode.append_attribute("width").set_value(GetImageWidth(tileset));
    imageNode.append_attribute("height").set_value(GetImageHeight(tileset));
  }

  AppendFancyTiles(node, tileset, dir);
  AppendProperties(node, tileset, dir);
}

void AppendEmbeddedTileset(pugi::xml_node mapNode,
                           const Tileset& tileset,
                           const std::filesystem::path& dir)
{
  auto node = mapNode.append_child("tileset");
  node.append_attribute("firstgid").set_value(GetFirstGlobalId(tileset));

  AddCommon(node, tileset, dir);
}

void CreateExternalTilesetFile(const Tileset& tileset,
                               const std::string& source,
                               const std::filesystem::path& dir)
{
  pugi::xml_document xml;

  auto root = xml.append_child("tileset");
  root.append_attribute("version").set_value(tiled_xml_version);
  root.append_attribute("tiledversion").set_value(tiled_version);

  AddCommon(root, tileset, dir);

  const auto path = dir / source;
  SaveXml(xml, path);
}

void AppendExternalTileset(pugi::xml_node mapNode,
                           const Tileset& tileset,
                           const std::string& source)
{
  auto node = mapNode.append_child("tileset");
  node.append_attribute("firstgid").set_value(GetFirstGlobalId(tileset));
  node.append_attribute("source").set_value(source.c_str());
}

}  // namespace

void AppendTileset(pugi::xml_node mapNode,
                   const Tileset& tileset,
                   const std::filesystem::path& dir,
                   const EmitterOptions options)
{
  if (options & EmitterOption_EmbedTilesets) {
    AppendEmbeddedTileset(mapNode, tileset, dir);
  }
  else {
    const auto source = fmt::format("{}.tsx", GetName(tileset));
    CreateExternalTilesetFile(tileset, source, dir);
    AppendExternalTileset(mapNode, tileset, source);
  }
}

}  // namespace tactile::IO
