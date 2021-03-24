#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class set_layer_visibility final : public QUndoCommand
{
 public:
  set_layer_visibility(not_null<core::map_document*> document,
                       layer_id id,
                       bool visible);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::set_layer_visibility);
  }

 private:
  not_null<core::map_document*> m_document{};
  layer_id m_id;
  bool m_visible;
};

}  // namespace tactile::cmd
