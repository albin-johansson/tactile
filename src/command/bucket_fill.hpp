#pragma once

#include "abstract_command.hpp"
#include "command_id.hpp"

namespace tactile::cmd {

class bucket_fill final : public abstract_command
{
 public:
  /**
   * @brief Creates a `bucket_fill` instance.
   *
   * @param map the associated map.
   * @param position the origin position of the flood fill.
   * @param target the target tile ID to replace.
   * @param replacement the tile ID that will be used to replace `target` tiles.
   *
   * @since 0.1.0
   */
  explicit bucket_fill(core::map* map,
                       const core::position& position,
                       tile_id target,
                       tile_id replacement);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::bucket_fill);
  }

 private:
  core::position m_position;
  tile_id m_target;
  tile_id m_replacement;
};

}  // namespace tactile::cmd
