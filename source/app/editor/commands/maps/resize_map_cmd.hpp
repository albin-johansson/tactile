#pragma once

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "map_command_cache.hpp"
#include "tactile.hpp"

namespace tactile {

class resize_map_cmd final : public command_base {
 public:
  resize_map_cmd(registry_ref registry, usize nRows, usize nCols);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::resize_map;
  }

 private:
  registry_ref mRegistry;
  usize mRows{};
  usize mCols{};
  map_command_cache mCache;
  maybe<usize> mPrevRows{};
  maybe<usize> mPrevCols{};

  [[nodiscard]] auto is_lossy_resize() const -> bool;
};

}  // namespace tactile
