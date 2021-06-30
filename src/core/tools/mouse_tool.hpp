#pragma once

#include "core/map/map_position.hpp"
#include "mouse_tool_type.hpp"

namespace Tactile {

class AMouseTool
{
 public:
  virtual ~AMouseTool() noexcept = default;

  virtual void Enable()
  {}

  virtual void Disable()
  {}

  virtual void OnPressed()
  {}

  virtual void OnMoved()
  {}

  virtual void OnReleased()
  {}

  virtual void OnEntered()
  {}

  virtual void OnExited()
  {}

  [[nodiscard]] virtual auto GetType() const -> MouseToolType = 0;
};

}  // namespace Tactile
