#include "remove_layer.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

remove_layer::remove_layer(core::map_document* document, layer_id id)
    : QUndoCommand{QStringLiteral(u"Remove Layer")},
      m_document{document},
      m_id{id}
{
  if (!m_document) {
    throw tactile_error{"Cannot create command from null map document!"};
  }
}

void remove_layer::undo()
{
  QUndoCommand::undo();
  m_document->add_layer(m_id, m_layer);
  while (m_document->index_of_layer(m_id).value() != m_index) {
    if (m_document->index_of_layer(m_id).value() < m_index) {
      m_document->move_layer_back(m_id);
    } else {
      m_document->move_layer_forward(m_id);
    }
  }
}

void remove_layer::redo()
{
  QUndoCommand::redo();
  m_index = m_document->index_of_layer(m_id).value();
  m_layer = m_document->take_layer(m_id);
  emit m_document->removed_layer(m_id);
}

}  // namespace tactile::cmd
