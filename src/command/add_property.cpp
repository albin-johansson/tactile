#include "add_property.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

add_property::add_property(core::property_manager* manager,
                           QString name,
                           const core::property::type type)
    : QUndoCommand{TACTILE_QSTRING(u"Add Property \"") + name +
                   TACTILE_QSTRING(u"\"")}
    , m_manager{manager}
    , m_name{std::move(name)}
    , m_type{type}
{
  if (!m_manager) {
    throw tactile_error{"Null property manager!"};
  }
}

void add_property::undo()
{
  QUndoCommand::undo();
  m_manager->remove_property(m_name);
}

void add_property::redo()
{
  QUndoCommand::redo();
  m_manager->add_property(m_name, m_type);
}

}  // namespace tactile::cmd
