#pragma once

#include <vector_map.hpp>  // vector_map

#include "aliases/tile_id.hpp"
#include "core/map/map_position.hpp"
#include "mouse_tool.hpp"

namespace Tactile {

class EraserTool final : public AMouseTool
{
 public:
  explicit EraserTool(NotNull<Model*> model);

  void OnPressed(const MouseInfo& info) override;

  void OnDragged(const MouseInfo& info) override;

  [[nodiscard]] auto GetType() const -> MouseToolType override
  {
    return MouseToolType::Eraser;
  }
  void OnReleased(const MouseInfo& info) override;

 private:
  rune::vector_map<MapPosition, tile_id> mOldState;

  void UpdateSequence(const MapPosition& position);

  [[nodiscard]] auto IsUsable() const -> bool;
};

}  // namespace Tactile
