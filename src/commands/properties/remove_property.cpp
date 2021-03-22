#include "remove_property.hpp"

#include <utility>  // move

#include "property_manager.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

remove_property::remove_property(core::property_manager* manager, QString name)
    : QUndoCommand{TACTILE_QSTRING(u"Remove Property \"") + name +
                   TACTILE_QSTRING(u"\"")}
    , m_manager{manager}
    , m_name{std::move(name)}
{
  if (!m_manager)
  {
    throw tactile_error{"Cannot create remove_property from null manager!"};
  }
}

void remove_property::undo()
{
  QUndoCommand::undo();

  const auto& property = m_property.value();
  m_manager->add_property(m_name, property.type().value());
  m_manager->set_property(m_name, property);

  m_property.reset();
}

void remove_property::redo()
{
  QUndoCommand::redo();

  m_property = m_manager->get_property(m_name);
  m_manager->remove_property(m_name);
}

}  // namespace tactile::cmd
