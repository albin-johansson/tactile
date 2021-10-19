#include "append_fancy_tiles.hpp"

#include "../../common_saving.hpp"
#include "append_properties.hpp"

namespace Tactile::IO {
namespace {

void AppendAnimation(pugi::xml_node tileNode, const std::vector<FrameData>& animation)
{
  auto animationNode = tileNode.append_child("animation");
  for (const auto& frame : animation) {
    auto frameNode = animationNode.append_child("frame");
    frameNode.append_attribute("tileid").set_value(frame.tile);
    frameNode.append_attribute("duration").set_value(frame.duration);
  }
}

}  // namespace

void AppendFancyTiles(pugi::xml_node node,
                      const std::vector<TileData>& tiles,
                      const std::filesystem::path& dir)
{
  for (const auto& tile : tiles) {
    if (IsTileWorthSaving(tile)) {
      auto tileNode = node.append_child("tile");
      tileNode.append_attribute("id").set_value(tile.id);

      if (!tile.animation.empty()) {
        AppendAnimation(tileNode, tile.animation);
      }

      AppendProperties(tileNode, tile.properties, dir);
    }
  }
}

}  // namespace Tactile::IO
