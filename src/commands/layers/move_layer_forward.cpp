#include "move_layer_forward.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

move_layer_forward::move_layer_forward(not_null<core::map_document*> document,
                                       const layer_id id)
    : QUndoCommand{QTranslator::tr("Move Layer Up")}
    , m_document{document}
    , m_id{id}
{
  if (!m_document)
  {
    throw tactile_error{"Null map document!"};
  }
}

void move_layer_forward::undo()
{
  QUndoCommand::undo();

  auto& map = m_document->raw();
  map.move_layer_back(m_id);

  emit m_document->moved_layer_back(m_id);
  emit m_document->redraw();
}

void move_layer_forward::redo()
{
  QUndoCommand::redo();

  auto& map = m_document->raw();
  map.move_layer_forward(m_id);

  emit m_document->moved_layer_forward(m_id);
  emit m_document->redraw();
}

}  // namespace tactile::cmd
