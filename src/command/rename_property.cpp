#include "rename_property.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

rename_property::rename_property(core::property_manager* manager,
                                 QString oldName,
                                 QString newName)
    : QUndoCommand{TACTILE_QSTRING(u"Rename Property \"") + oldName +
                   TACTILE_QSTRING(u"\" to \"") + newName +
                   TACTILE_QSTRING(u"\"")}
    , m_manager{manager}
    , m_oldName{std::move(oldName)}
    , m_newName{std::move(newName)}
{
  if (!m_manager) {
    throw tactile_error{"Null property manager!"};
  }
}

void rename_property::undo()
{
  QUndoCommand::undo();
  m_manager->rename_property(m_newName, m_oldName);
}

void rename_property::redo()
{
  QUndoCommand::redo();
  m_manager->rename_property(m_oldName, m_newName);
}

}  // namespace tactile::cmd
