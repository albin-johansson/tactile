#include "append_fancy_tiles.hpp"

#include "../../common_saving.hpp"
#include "append_object.hpp"
#include "append_properties.hpp"

namespace Tactile::IO {
namespace {

void AppendAnimation(pugi::xml_node tileNode, const Tile& tile)
{
  auto animationNode = tileNode.append_child("animation");

  EachAnimationFrame(tile, [&](const AnimationFrame& frame) {
    auto frameNode = animationNode.append_child("frame");
    frameNode.append_attribute("tileid").set_value(GetTile(frame));
    frameNode.append_attribute("duration").set_value(GetDuration(frame));
  });
}

void AppendObjects(pugi::xml_node tileNode,
                   const Tile& tile,
                   const std::filesystem::path& dir)
{
  auto objectLayerNode = tileNode.append_child("objectgroup");
  objectLayerNode.append_attribute("draworder").set_value("index");

  EachObject(tile,
             [&](const Object& object) { AppendObject(objectLayerNode, object, dir); });
}

}  // namespace

void AppendFancyTiles(pugi::xml_node node,
                      const Tileset& tileset,
                      const std::filesystem::path& dir)
{
  EachTileInfo(tileset, [&](const Tile& tile) {
    if (IsTileWorthSaving(tile)) {
      auto tileNode = node.append_child("tile");
      tileNode.append_attribute("id").set_value(GetId(tile));

      if (GetAnimationFrameCount(tile) != 0) {
        AppendAnimation(tileNode, tile);
      }

      if (GetObjectCount(tile) != 0) {
        AppendObjects(tileNode, tile, dir);
      }

      AppendProperties(tileNode, tile, dir);
    }
  });
}

}  // namespace Tactile::IO
