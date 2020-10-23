#include "remove_layer.hpp"

#include <utility>  // move

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
  m_document->add_layer(m_id, m_layer);
  // TODO restore actual position of layer
}

void remove_layer::redo()
{
  m_layer = m_document->take_layer(m_id);
  emit m_document->removed_layer(m_id);
}

}  // namespace tactile::cmd
