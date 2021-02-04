#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "czstring.hpp"
#include "map.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "tileset_manager.hpp"
#include "vector_map.hpp"

namespace tactile::cmd {

class stamp_sequence final : public QUndoCommand
{
 public:
  stamp_sequence(core::map* map,
                 vector_map<core::position, tile_id>&& oldState,
                 vector_map<core::position, tile_id>&& sequence);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::stamp);
  }

 private:
  core::map* m_map{};
  vector_map<core::position, tile_id> m_oldState;
  vector_map<core::position, tile_id> m_sequence;
  layer_id m_layer{};
  bool m_first{true};
};

}  // namespace tactile::cmd
