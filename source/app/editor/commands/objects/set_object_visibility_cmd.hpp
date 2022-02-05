#pragma once

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"
#include "tactile.hpp"

namespace tactile {

class set_object_visibility_cmd final : public object_cmd {
 public:
  set_object_visibility_cmd(registry_ref registry, object_id id, bool visible);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::set_object_visibility;
  }

 private:
  bool mVisible;
  maybe<bool> mPreviousVisibility;
};

}  // namespace tactile
