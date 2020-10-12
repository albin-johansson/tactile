#pragma once

#include "abstract_command.hpp"
#include "command_id.hpp"
#include "position.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace tactile::cmd {

class erase_sequence final : public abstract_command
{
 public:
  erase_sequence(core::map* map,
                 vector_map<core::position, tile_id>&& oldState);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::eraser);
  }

 private:
  vector_map<core::position, tile_id> m_oldState;
  layer_id m_layer{};
  bool m_first{true};
};

}  // namespace tactile::cmd
