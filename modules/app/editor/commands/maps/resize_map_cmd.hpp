#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "map_command_cache.hpp"
#include "tactile_def.hpp"

namespace tactile {

class ResizeMapCmd final : public ACommand {
 public:
  ResizeMapCmd(RegistryRef registry, usize nRows, usize nCols);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::ResizeMap;
  }

 private:
  RegistryRef mRegistry;
  usize mRows{};
  usize mCols{};
  MapCommandCache mCache;
  maybe<usize> mPrevRows{};
  maybe<usize> mPrevCols{};

  [[nodiscard]] auto IsLossyResize() const -> bool;
};

}  // namespace tactile
