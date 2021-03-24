#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "tileset_id.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class set_tileset_name final : public QUndoCommand
{
 public:
  set_tileset_name(not_null<core::map_document*> document,
                   tileset_id id,
                   QString name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::set_tileset_name);
  }

 private:
  core::map_document* m_document{};
  tileset_id m_id;
  QString m_name;
  maybe<QString> m_previous;
};

}  // namespace tactile::cmd
