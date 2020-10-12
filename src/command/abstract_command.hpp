#pragma once

#include <QString>
#include <QUndoCommand>

#include "map.hpp"
#include "types.hpp"

namespace tactile::cmd {

class abstract_command : public QUndoCommand
{
 protected:
  explicit abstract_command(const QString& text, core::map* map);

  core::map* m_map{};
};

}  // namespace tactile::cmd
