#include "add_layer.hpp"

#include <utility>  // move

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

add_layer::add_layer(core::map_document* document,
                     shared<core::layer> layer,
                     const layer_id id)
    : QUndoCommand{TACTILE_QSTRING(u"Add Layer")}
    , m_document{document}
    , m_layer{std::move(layer)}
    , m_id{id}
{
  if (!m_document) {
    throw tactile_error{"Cannot create command from null map document!"};
  }

  if (!m_layer) {
    throw tactile_error{"Cannot create command from null tile_layer!"};
  }
}

void add_layer::undo()
{
  QUndoCommand::undo();
  m_document->take_layer(m_id);
  emit m_document->removed_layer(m_id);
}

void add_layer::redo()
{
  QUndoCommand::redo();
  m_document->add_layer(m_id, m_layer);
}

}  // namespace tactile::cmd
