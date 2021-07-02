#pragma once

#include <vector_map.hpp>  // vector_map

#include "aliases/tile_id.hpp"
#include "core/map/map_position.hpp"
#include "mouse_tool.hpp"

namespace Tactile {

class MapDocument;
class Tileset;

class StampTool final : public AMouseTool
{
 public:
  explicit StampTool(NotNull<Model*> model);

  void OnPressed(const MouseInfo& info) override;

  void OnDragged(const MouseInfo& info) override;

  void OnReleased(const MouseInfo& info) override;

  [[nodiscard]] auto GetType() const -> MouseToolType override
  {
    return MouseToolType::Stamp;
  }

 private:
  rune::vector_map<MapPosition, tile_id> mOldState;
  rune::vector_map<MapPosition, tile_id> mSequence;

  [[nodiscard]] auto IsUsable() const -> bool;

  void UpdateSequence(const Tileset& tileset, const MapPosition& origin);
};

}  // namespace Tactile
