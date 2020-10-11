#include "abstract_command.hpp"

namespace tactile::cmd {

abstract_command::abstract_command(const QString& text, core::map* map)
    : QUndoCommand{text},
      m_map{map}
{}

}  // namespace tactile::cmd
