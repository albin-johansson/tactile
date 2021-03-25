#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "position.hpp"
#include "tile_id.hpp"
#include "vector_map.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class stamp_sequence final : public QUndoCommand
{
 public:
  stamp_sequence(not_null<core::map_document*> document,
                 vector_map<core::position, tile_id>&& oldState,
                 vector_map<core::position, tile_id>&& sequence);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::stamp);
  }

 private:
  core::map_document* m_document{};
  vector_map<core::position, tile_id> m_oldState;
  vector_map<core::position, tile_id> m_sequence;
  layer_id m_layer{};
  bool m_first{true};
};

}  // namespace tactile::cmd
