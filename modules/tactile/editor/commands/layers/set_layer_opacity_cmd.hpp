#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetLayerOpacityCmd final : public ACommand {
 public:
  SetLayerOpacityCmd(RegistryRef registry, LayerID id, float opacity);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetLayerOpacity;
  }

 private:
  RegistryRef mRegistry;
  LayerID mLayerId;
  float mOpacity;
  Maybe<float> mPreviousOpacity;
};

}  // namespace tactile
