#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "ints.hpp"
#include "layer.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "remove_layer.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class RemoveLayer final : public QUndoCommand
{
 public:
  RemoveLayer(not_null<core::map_document*> document, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::remove_layer);
  }

 private:
  core::map_document* mDocument{};
  shared<core::layer> mLayer;
  layer_id mId;
  usize mIndex{};
};

}  // namespace tactile::cmd
