#pragma once

#include <QString>
#include <QUndoCommand>

#include "tilemap.hpp"
#include "tactile_types.hpp"

namespace tactile::cmd {

class abstract_command : public QUndoCommand {
 protected:
  explicit abstract_command(const QString& text,
                            not_null<model::tilemap*> map) ;

  model::tilemap* m_map;
};

}  // namespace tactile::cmd
