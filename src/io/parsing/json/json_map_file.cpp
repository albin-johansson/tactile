#include "json_map_file.hpp"

namespace Tactile::IO {

JsonMapFile::JsonMapFile(const std::filesystem::path& path) : mRoot{path}
{}

auto JsonMapFile::GetMap() const -> Object
{
  return std::make_unique<JsonMapObject>(mRoot);
}

auto JsonMapFile::GetLayers() const -> Objects
{
  return mRoot.GetArray("layers");
}

auto JsonMapFile::GetTilesets() const -> Objects
{
  return mRoot.GetArray("tilesets");
}

auto JsonMapFile::ContainsLayers() const -> bool
{
  const auto& json = mRoot.GetData();
  return json.find("layers") != json.end();
}

auto JsonMapFile::ContainsTilesets() const -> bool
{
  const auto& json = mRoot.GetData();
  return json.find("tilesets") != json.end();
}

}  // namespace Tactile::IO
