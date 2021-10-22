#include "append_fancy_tiles.hpp"

#include "../../common_saving.hpp"
#include "append_object.hpp"
#include "append_properties.hpp"

namespace Tactile::IO {
namespace {

void AppendAnimation(pugi::xml_node tileNode, const Tile& tile, const usize nFrames)
{
  auto animationNode = tileNode.append_child("animation");

  for (usize index = 0; index < nFrames; ++index) {
    const auto& frame = GetAnimationFrame(tile, index);
    auto frameNode = animationNode.append_child("frame");
    frameNode.append_attribute("tileid").set_value(GetTile(frame));
    frameNode.append_attribute("duration").set_value(GetDuration(frame));
  }
}

void AppendObjects(pugi::xml_node tileNode,
                   const Tile& tile,
                   const usize nObjects,
                   const std::filesystem::path& dir)
{
  auto objectLayerNode = tileNode.append_child("objectgroup");
  objectLayerNode.append_attribute("draworder").set_value("index");

  for (usize index = 0; index < nObjects; ++index) {
    const auto& object = GetObject(tile, index);
    AppendObject(objectLayerNode, object, dir);
  }
}

}  // namespace

void AppendFancyTiles(pugi::xml_node node,
                      const Tileset& tileset,
                      const std::filesystem::path& dir)
{
  const auto nTiles = GetTileInfoCount(tileset);
  for (usize index = 0; index < nTiles; ++index) {
    const auto& tile = GetTileInfo(tileset, index);

    if (IsTileWorthSaving(tile)) {
      auto tileNode = node.append_child("tile");
      tileNode.append_attribute("id").set_value(GetId(tile));

      if (const auto nFrames = GetAnimationFrameCount(tile); nFrames != 0) {
        AppendAnimation(tileNode, tile, nFrames);
      }

      if (const auto nObjects = GetObjectCount(tile); nObjects != 0) {
        AppendObjects(tileNode, tile, nObjects, dir);
      }

      AppendProperties(tileNode, tile, dir);
    }
  }
}

}  // namespace Tactile::IO
