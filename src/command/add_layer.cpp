#include "add_layer.hpp"

#include <utility>  // move

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

add_layer::add_layer(core::map_document* document,
                     std::shared_ptr<core::layer> layer,
                     layer_id id)
    : QUndoCommand{QStringLiteral(u"Add Layer")},
      m_document{document},
      m_layer{std::move(layer)},
      m_id{id}
{
  if (!m_document) {
    throw tactile_error{"Cannot create command from null map document!"};
  }

  if (!m_layer) {
    throw tactile_error{"Cannot create command from null layer!"};
  }
}

void add_layer::undo()
{
  m_document->take_layer(m_id);
  emit m_document->removed_layer(m_id);
}

void add_layer::redo()
{
  m_document->add_layer(m_id, m_layer);
}

}  // namespace tactile::cmd
