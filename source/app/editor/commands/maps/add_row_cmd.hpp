#pragma once

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class add_row_cmd final : public command_base
{
 public:
  explicit add_row_cmd(registry_ref registry);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const command_base& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override { return command_id::add_row; }

 private:
  registry_ref mRegistry;
  usize mRows{1};
};

}  // namespace tactile
