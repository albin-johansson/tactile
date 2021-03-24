#include "set_layer_visibility.hpp"

#include <QTranslator>  // tr

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

set_layer_visibility::set_layer_visibility(
    not_null<core::map_document*> document,
    const layer_id id,
    const bool visible)
    : QUndoCommand{QTranslator::tr("Set Layer Visibility")}
    , m_document{document}
    , m_visible{visible}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void set_layer_visibility::undo()
{
  QUndoCommand::undo();

  auto& map = m_document->raw();
  map.set_visibility(m_id, !m_visible);

  emit m_document->changed_layer_visibility(m_id, !m_visible);
  emit m_document->redraw();
}

void set_layer_visibility::redo()
{
  QUndoCommand::redo();

  auto& map = m_document->raw();
  map.set_visibility(m_id, m_visible);

  emit m_document->changed_layer_visibility(m_id, m_visible);
  emit m_document->redraw();
}

}  // namespace tactile::cmd
