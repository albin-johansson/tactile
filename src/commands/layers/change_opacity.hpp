#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class change_opacity final : public QUndoCommand
{
 public:
  change_opacity(not_null<core::map_document*> document,
                 layer_id id,
                 double opacity);

  void undo() override;

  void redo() override;

  auto mergeWith(const QUndoCommand* other) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::change_opacity);
  }

 private:
  core::map_document* m_document{};
  layer_id m_id;
  double m_opacity;
  maybe<double> m_previousOpacity;
};

}  // namespace tactile::cmd
