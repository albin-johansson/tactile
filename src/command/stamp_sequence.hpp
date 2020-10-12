#pragma once

#include "abstract_command.hpp"
#include "command_id.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "tileset_manager.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace tactile::cmd {

class stamp_sequence final : public abstract_command
{
 public:
  stamp_sequence(core::map* map,
                 vector_map<core::position, tile_id>&& oldState,
                 vector_map<core::position, tile_id>&& sequence);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::stamp);
  }

 private:
  vector_map<core::position, tile_id> m_oldState;
  vector_map<core::position, tile_id> m_sequence;
  bool m_first{true};
};

}  // namespace tactile::cmd
