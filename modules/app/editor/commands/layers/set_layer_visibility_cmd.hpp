#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetLayerVisibilityCmd final : public command_base {
 public:
  SetLayerVisibilityCmd(registry_ref registry, layer_id id, bool visible);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::SetLayerVisible;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayerId;
  bool mVisible;
  maybe<bool> mPreviousVisibility;
};

}  // namespace tactile