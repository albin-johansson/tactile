#include "xml_map_file.hpp"

#include "core/tactile_error.hpp"

namespace Tactile::IO {

XmlMapFile::XmlMapFile(const std::filesystem::path& path)
    : mDocument{std::make_shared<pugi::xml_document>()}
{
  if (const auto result = mDocument->load_file(path.c_str()); !result)
  {
    throw TactileError{"Could not parse XML map file!"};
  }
}

auto XmlMapFile::GetMap() const -> Object
{
  return std::make_unique<XmlMapObject>(GetMapNode());
}

auto XmlMapFile::GetLayers() const -> Objects
{
  const auto node = GetMapNode();
  return node.GetLayers();
}

auto XmlMapFile::GetTilesets() const -> Objects
{
  const auto node = GetMapNode();
  return node.GetArray(MapAttribute::Tileset);
}

auto XmlMapFile::ContainsLayers() const -> bool
{
  // XML maps don't feature a "layers" node, so return true to avoid false errors
  return true;
}

auto XmlMapFile::ContainsTilesets() const -> bool
{
  // XML maps don't feature a "tilesets" node, so return true to avoid false errors
  return true;
}

auto XmlMapFile::GetMapNode() const -> XmlMapObject
{
  return XmlMapObject{mDocument, mDocument->child("map")};
}

}  // namespace Tactile::IO
