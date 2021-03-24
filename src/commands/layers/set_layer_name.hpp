#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class set_layer_name final : public QUndoCommand
{
 public:
  set_layer_name(not_null<core::map_document*> document,
                 layer_id id,
                 QString name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::set_layer_name);
  }

 private:
  not_null<core::map_document*> m_document{};
  layer_id m_id;
  QString m_name;
  maybe<QString> m_previous;
};

}  // namespace tactile::cmd
