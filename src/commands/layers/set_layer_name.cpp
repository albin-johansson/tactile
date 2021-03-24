#include "set_layer_name.hpp"

#include <QTranslator>  // tr
#include <utility>      // move

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

set_layer_name::set_layer_name(not_null<core::map_document*> document,
                               const layer_id id,
                               QString name)
    : QUndoCommand{QTranslator::tr("Set Layer Name")}
    , m_document{document}
    , m_id{id}
    , m_name{std::move(name)}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void set_layer_name::undo()
{
  QUndoCommand::undo();

  auto& map = m_document->raw();
  map.set_name(m_id, m_previous.value());

  emit m_document->changed_layer_name(m_id, m_previous.value());
  emit m_document->redraw();

  m_previous.reset();
}

void set_layer_name::redo()
{
  QUndoCommand::redo();

  auto& map = m_document->raw();

  m_previous = map.name(m_id);
  map.set_name(m_id, m_name);

  emit m_document->changed_layer_name(m_id, m_name);
  emit m_document->redraw();
}

}  // namespace tactile::cmd
