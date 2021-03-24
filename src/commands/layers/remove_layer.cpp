#include "remove_layer.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

remove_layer::remove_layer(core::map_document* document, const layer_id id)
    : QUndoCommand{TACTILE_QSTRING(u"Remove Layer")}
    , m_document{document}
    , m_id{id}
{
  if (!m_document)
  {
    throw tactile_error{"Null map document!"};
  }
}

void remove_layer::undo()
{
  QUndoCommand::undo();

  auto& map = m_document->raw();

  map.add_layer(m_id, m_layer);
  while (map.index_of(m_id).value() != m_index)
  {
    if (map.index_of(m_id).value() < m_index)
    {
      map.move_layer_back(m_id);
    }
    else
    {
      map.move_layer_forward(m_id);
    }
  }

  emit m_document->added_layer(m_id, *m_layer);
  emit m_document->redraw();
}

void remove_layer::redo()
{
  QUndoCommand::redo();

  auto& map = m_document->raw();

  m_index = map.index_of(m_id).value();
  m_layer = map.take_layer(m_id);

  emit m_document->removed_layer(m_id);
  emit m_document->redraw();
}

}  // namespace tactile::cmd
