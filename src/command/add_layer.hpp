#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer.hpp"
#include "layer_id.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class add_layer final : public QUndoCommand
{
 public:
  add_layer(core::map_document* document,
            shared<core::layer> layer,
            layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::add_layer);
  }

 private:
  core::map_document* m_document{};
  shared<core::layer> m_layer;
  layer_id m_id;
};

}  // namespace tactile::cmd
