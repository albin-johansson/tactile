#pragma once

#include <utility>  // pair, move
#include <vector>

#include "abstract_command.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "tileset_manager.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace tactile::cmd {

class stamp_sequence final : public abstract_command
{
 public:
  stamp_sequence(not_null<core::map*> map,
                 vector_map<core::position, tile_id>&& oldState,
                 vector_map<core::position, tile_id>&& sequence);

  void undo() override;

  void redo() override;

 private:
  vector_map<core::position, tile_id> m_oldState;
  vector_map<core::position, tile_id> m_sequence;
  bool m_first{true};
};

}  // namespace tactile::cmd
