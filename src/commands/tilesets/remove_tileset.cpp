#include "remove_tileset.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

remove_tileset::remove_tileset(core::map_document* document,
                               shared<core::tileset> tileset,
                               const tileset_id id)
    : QUndoCommand{TACTILE_QSTRING(u"Remove Tileset")}
    , m_document{document}
    , m_tileset{std::move(tileset)}
    , m_id{id}
{
  if (!m_document) {
    throw tactile_error{"Null map document!"};
  }

  if (!m_tileset) {
    throw tactile_error{"Null tileset!"};
  }
}

void remove_tileset::undo()
{
  QUndoCommand::undo();
  m_document->add_tileset(m_id, m_tileset);
}

void remove_tileset::redo()
{
  QUndoCommand::redo();
  m_document->remove_tileset(m_id);
}

}  // namespace tactile::cmd
