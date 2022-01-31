#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "map_command_cache.hpp"
#include "tactile_def.hpp"

namespace tactile {

class ResizeMapCmd final : public command_base {
 public:
  ResizeMapCmd(registry_ref registry, usize nRows, usize nCols);

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
  MapCommandCache mCache;
  maybe<usize> mPrevRows{};
  maybe<usize> mPrevCols{};

  [[nodiscard]] auto IsLossyResize() const -> bool;
};

}  // namespace tactile
