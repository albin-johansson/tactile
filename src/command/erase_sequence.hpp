#pragma once

#include <QUndoCommand>

#include "command_id.hpp"
#include "czstring.hpp"
#include "map.hpp"
#include "position.hpp"
#include "vector_map.hpp"

namespace tactile::cmd {

class erase_sequence final : public QUndoCommand
{
 public:
  erase_sequence(core::map* map,
                 vector_map<core::position, tile_id>&& oldState);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::eraser);
  }

 private:
  core::map* m_map{};
  vector_map<core::position, tile_id> m_oldState;
  layer_id m_layer{};
  bool m_first{true};
};

}  // namespace tactile::cmd
