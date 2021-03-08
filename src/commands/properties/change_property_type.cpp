#include "change_property_type.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

// clang-format off

change_property_type::change_property_type(not_null<core::property_manager*> manager,
                                           QString name,
                                           const core::property_type type)
    : QUndoCommand{TACTILE_QSTRING(u"Change Property Type")}
    , m_manager{manager}
    , m_name{std::move(name)}
    , m_type{type}
{
  if (!m_manager) {
    throw tactile_error{"Null property manager!"};
  }
}

// clang-format on

void change_property_type::undo()
{
  QUndoCommand::undo();

  const auto& prev = m_previousProperty.value();
  m_manager->change_property_type(m_name, prev.type().value());
  m_manager->set_property(m_name, prev);
  m_previousProperty.reset();
}

void change_property_type::redo()
{
  QUndoCommand::redo();

  m_previousProperty = m_manager->get_property(m_name);
  m_manager->change_property_type(m_name, m_type);
}

}  // namespace tactile::cmd
