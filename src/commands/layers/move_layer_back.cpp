#include "move_layer_back.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

move_layer_back::move_layer_back(not_null<core::map_document*> document,
                                 const layer_id id)
    : QUndoCommand{QTranslator::tr("Move Layer Back")}
    , m_document{document}
    , m_id{id}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void move_layer_back::undo()
{
  QUndoCommand::undo();

  auto& map = m_document->raw();
  map.move_layer_forward(m_id);

  emit m_document->moved_layer_forward(m_id);
  emit m_document->redraw();
}

void move_layer_back::redo()
{
  QUndoCommand::redo();

  auto& map = m_document->raw();
  map.move_layer_back(m_id);

  emit m_document->moved_layer_back(m_id);
  emit m_document->redraw();
}

}  // namespace tactile::cmd
