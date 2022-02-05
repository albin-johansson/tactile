#pragma once

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class set_layer_opacity_cmd final : public command_base
{
 public:
  set_layer_opacity_cmd(registry_ref registry, layer_id id, float opacity);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const command_base& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::set_layer_opacity;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayerId;
  float mOpacity;
  maybe<float> mPreviousOpacity;
};

}  // namespace tactile
