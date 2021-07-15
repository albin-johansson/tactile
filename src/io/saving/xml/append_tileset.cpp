#include "append_tileset.hpp"

#include <format>  // format

#include "io/preferences.hpp"
#include "io/saving/common.hpp"
#include "io/saving/xml/append_properties.hpp"
#include "io/saving/xml/save_xml.hpp"

namespace Tactile::IO {
namespace {

void AddCommon(const Tileset& tileset,
               pugi::xml_node& node,
               const std::filesystem::path& dir)
{
  node.append_attribute("name").set_value(tileset.GetName().data());
  node.append_attribute("tilewidth").set_value(tileset.GetTileWidth());
  node.append_attribute("tileheight").set_value(tileset.GetTileHeight());
  node.append_attribute("tilecount").set_value(tileset.GetTileCount());
  node.append_attribute("columns").set_value(tileset.GetColumnCount().get());

  auto imageNode = node.append_child("image");
  const auto source = GetTilesetImagePath(tileset, dir);
  imageNode.append_attribute("source").set_value(source.c_str());
  imageNode.append_attribute("width").set_value(tileset.GetWidth());
  imageNode.append_attribute("height").set_value(tileset.GetHeight());

  AppendProperties(tileset, node, dir);
}

void AppendEmbeddedTileset(const Tileset& tileset,
                           pugi::xml_node& mapNode,
                           const std::filesystem::path& dir)
{
  auto node = mapNode.append_child("tileset");
  node.append_attribute("firstgid").set_value(tileset.GetFirstId().get());

  AddCommon(tileset, node, dir);
}

void CreateExternalTilesetFile(const Tileset& tileset,
                               const std::string& source,
                               const std::filesystem::path& dir)
{
  pugi::xml_document xml;

  auto root = xml.append_child("tileset");
  root.append_attribute("version").set_value(tiled_xml_version);
  root.append_attribute("tiledversion").set_value(tiled_version);

  AddCommon(tileset, root, dir);

  const auto path = dir / source;
  CENTURION_LOG_INFO("Saving external tileset in \"%s\"", path.string().c_str());

  SaveXml(xml, path);
}

void AppendExternalTileset(const Tileset& tileset,
                           const std::string& source,
                           pugi::xml_node& mapNode)
{
  auto node = mapNode.append_child("tileset");
  node.append_attribute("firstgid").set_value(tileset.GetFirstId().get());
  node.append_attribute("source").set_value(source.c_str());
}

}  // namespace

void AppendTileset(const Tileset& tileset,
                   pugi::xml_node& mapNode,
                   const std::filesystem::path& dir)
{
  if (Prefs::GetEmbedTilesets())
  {
    AppendEmbeddedTileset(tileset, mapNode, dir);
  }
  else
  {
    const auto source = std::format("{}.tsx", tileset.GetName());
    CreateExternalTilesetFile(tileset, source, dir);
    AppendExternalTileset(tileset, source, mapNode);
  }
}

}  // namespace Tactile::IO
