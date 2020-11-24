#include "add_tileset.hpp"

#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile::cmd {

add_tileset::add_tileset(core::map_document* document,
                         std::shared_ptr<core::tileset> tileset,
                         const tileset_id id)
    : QUndoCommand{QStringLiteral(u"Add Tileset")},
      m_document{document},
      m_tileset{std::move(tileset)},
      m_id{id}
{
  if (!m_document) {
    throw tactile_error{
        "Cannot create add_tileset command with null document!"};
  }

  if (!m_tileset) {
    throw tactile_error{"Cannot create add_tileset command with null tileset!"};
  }
}

void add_tileset::undo()
{
  QUndoCommand::undo();
  m_document->remove_tileset(m_id);
}

void add_tileset::redo()
{
  QUndoCommand::redo();
  m_document->add_tileset(m_id, m_tileset);
}

}  // namespace tactile::cmd
