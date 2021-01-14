#include "add_property.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

add_property::add_property(core::property_manager* manager,
                           QString name,
                           const core::property::type type)
    : QUndoCommand{TACTILE_QSTRING(u"Add property")}
    , m_manager{manager}
    , m_name{std::move(name)}
    , m_type{type}
{
  if (!m_manager) {
    throw tactile_error{"Can't create add_property command with null manager!"};
  }
}

void add_property::undo()
{
  QUndoCommand::undo();
  m_manager->remove_property(m_name);
  m_manager->notify_property_removed(m_name);
}

void add_property::redo()
{
  QUndoCommand::redo();
  m_manager->add_property(m_name, m_type);
  if (!m_first) {
    m_manager->notify_property_added(m_name);
  } else {
    m_first = false;
  }
}

}  // namespace tactile::cmd
