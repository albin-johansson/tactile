#pragma once

#include <vector_map.hpp>  // vector_map

#include "aliases/tile_id.hpp"
#include "core/map/map_position.hpp"
#include "mouse_tool.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class EraserTool final : public AMouseTool
{
 public:
  explicit EraserTool(NotNull<Model*> model);

  void OnPressed(const MouseInfo& info) override;

  void OnDragged(const MouseInfo& info) override;

  void OnReleased(const MouseInfo& info) override;

  [[nodiscard]] auto GetType() const -> MouseToolType override
  {
    return MouseToolType::Eraser;
  }

 private:
  rune::vector_map<MapPosition, tile_id> mOldState;

  void UpdateSequence(const MapPosition& position);

  [[nodiscard]] auto IsUsable() const -> bool;
};

/// \} End of group core

}  // namespace Tactile
