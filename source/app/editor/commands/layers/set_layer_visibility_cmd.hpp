#pragma once

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class set_layer_visibility_cmd final : public command_base {
 public:
  set_layer_visibility_cmd(registry_ref registry, layer_id id, bool visible);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::set_layer_visible;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayerId;
  bool mVisible;
  maybe<bool> mPreviousVisibility;
};

}  // namespace tactile