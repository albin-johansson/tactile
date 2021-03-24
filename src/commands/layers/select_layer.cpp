#include "select_layer.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

select_layer::select_layer(not_null<core::map_document*> document,
                           const layer_id id)
    : QUndoCommand{QTranslator::tr("Select Layer")}
    , m_document{document}
    , m_id{id}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void select_layer::undo()
{
  QUndoCommand::undo();

  if (m_previous)
  {
    auto& map = m_document->raw();
    map.select_layer(*m_previous);

    const auto& layer = map.get_layer(m_id);
    Q_ASSERT(layer);

    emit m_document->selected_layer(*m_previous, *layer);

    m_previous.reset();
  }
}

void select_layer::redo()
{
  QUndoCommand::redo();

  auto& map = m_document->raw();

  m_previous = map.active_layer_id();
  map.select_layer(m_id);

  const auto& layer = map.get_layer(m_id);
  Q_ASSERT(layer);

  emit m_document->selected_layer(m_id, *layer);
}

}  // namespace tactile::cmd
