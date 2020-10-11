#include "remove_tileset.hpp"

#include "tactile_error.hpp"

namespace tactile::cmd {

remove_tileset::remove_tileset(core::map_document* document,
                               std::shared_ptr<core::tileset> tileset,
                               tileset_id id)
    : QUndoCommand{QStringLiteral(u"Remove Tileset")},
      m_document{document},
      m_tileset{std::move(tileset)},
      m_id{id}
{
  if (!m_document) {
    throw tactile_error{
        "Cannot create remove_tileset command with null document!"};
  }

  if (!m_tileset) {
    throw tactile_error{
        "Cannot create remove_tileset command with null tileset!"};
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

  // This command is associated with the "X"-button on tileset tabs, which means
  // that the tileset has already been removed from the UI the first time the
  // command is executed. Only emit signals when the command is executed as a
  // direct result of user undo/redo request.
  if (m_first) {
    m_document->remove_tileset(m_id, false);
    m_first = false;
  } else {
    m_document->remove_tileset(m_id);
  }
}

}  // namespace tactile::cmd
