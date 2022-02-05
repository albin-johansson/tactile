#pragma once

#include <string>  // string

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"
#include "tactile.hpp"

namespace tactile {

class set_object_name_cmd final : public object_cmd {
 public:
  set_object_name_cmd(registry_ref registry, object_id id, std::string name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const command_base& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::set_object_name;
  }

 private:
  std::string mNewName;
  maybe<std::string> mOldName;
};

}  // namespace tactile
