#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class select_layer final : public QUndoCommand
{
 public:
  select_layer(not_null<core::map_document*> document, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::select_layer);
  }

 private:
  core::map_document* m_document{};
  layer_id m_id;
  maybe<layer_id> m_previous;
};

}  // namespace tactile::cmd
