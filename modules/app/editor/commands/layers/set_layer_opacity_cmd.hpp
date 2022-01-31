#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetLayerOpacityCmd final : public command_base {
 public:
  SetLayerOpacityCmd(registry_ref registry, layer_id id, float opacity);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const command_base& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::SetLayerOpacity;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayerId;
  float mOpacity;
  maybe<float> mPreviousOpacity;
};

}  // namespace tactile
