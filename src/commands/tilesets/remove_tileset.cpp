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
  if (!m_document)
  {
    throw tactile_error{"Null map document!"};
  }

  if (!m_tileset)
  {
    throw tactile_error{"Null tileset!"};
  }
}

void remove_tileset::undo()
{
  QUndoCommand::undo();

  auto* tilesets = m_document->tilesets();
  tilesets->add(m_id, m_tileset);

  emit m_document->added_tileset(m_id);
}

void remove_tileset::redo()
{
  QUndoCommand::redo();

  auto& map = m_document->raw();
  auto* tilesets = m_document->tilesets();

  const auto [first, last] = tilesets->range_of(m_id);
  map.remove_occurrences(first, last);

  tilesets->remove(m_id);

  emit m_document->removed_tileset(m_id);
  emit m_document->redraw();
}

}  // namespace tactile::cmd
