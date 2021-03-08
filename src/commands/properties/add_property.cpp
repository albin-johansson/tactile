#include "add_property.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

add_property::add_property(not_null<core::property_manager*> manager,
                           QString name,
                           data_type data)
    : QUndoCommand{TACTILE_QSTRING(u"Add Property \"") + name +
                   TACTILE_QSTRING(u"\"")}
    , m_manager{manager}
    , m_name{std::move(name)}
    , m_data{std::move(data)}
{
  if (!m_manager) {
    throw tactile_error{"Null property manager!"};
  }
}

add_property::add_property(not_null<core::property_manager*> manager,
                           QString name,
                           const core::property& property)
    : add_property{manager, std::move(name), data_type{property}}
{}

add_property::add_property(not_null<core::property_manager*> manager,
                           QString name,
                           const core::property_type type)
    : add_property{manager, std::move(name), data_type{type}}
{}

void add_property::undo()
{
  QUndoCommand::undo();
  m_manager->remove_property(m_name);
}

void add_property::redo()
{
  QUndoCommand::redo();

  if (const auto* type = std::get_if<core::property_type>(&m_data)) {
    m_manager->add_property(m_name, *type);
  } else {
    m_manager->add_property(m_name, std::get<core::property>(m_data));
  }
}

}  // namespace tactile::cmd
