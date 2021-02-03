#include "update_property.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

update_property::update_property(core::property_manager* manager,
                                 QString name,
                                 core::property property)
    : QUndoCommand{TACTILE_QSTRING(u"Update Property \"") + name +
                   TACTILE_QSTRING(u"\"")}
    , m_manager{manager}
    , m_name{std::move(name)}
    , m_property{std::move(property)}
{
  if (!m_manager) {
    throw tactile_error{"Null property manager!"};
  }
}

void update_property::undo()
{
  QUndoCommand::undo();

  m_manager->set_property(m_name, m_prevProperty.value());
  m_prevProperty.reset();
}

void update_property::redo()
{
  QUndoCommand::redo();

  m_prevProperty = m_manager->get_property(m_name);
  m_manager->set_property(m_name, m_property);
}

}  // namespace tactile::cmd
