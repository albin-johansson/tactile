#include "set_tileset_name.hpp"

#include <QTranslator>  // tr
#include <utility>      // move

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

set_tileset_name::set_tileset_name(not_null<core::map_document*> document,
                                   const tileset_id id,
                                   QString name)
    : QUndoCommand{QTranslator::tr("Set Tileset Name")}
    , m_document{document}
    , m_id{id}
    , m_name{std::move(name)}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void set_tileset_name::undo()
{
  QUndoCommand::undo();

  const auto name = m_previous.value();

  auto* tilesets = m_document->tilesets();
  tilesets->rename(m_id, name);

  emit m_document->renamed_tileset(m_id, name);
  m_previous.reset();
}

void set_tileset_name::redo()
{
  QUndoCommand::redo();

  auto* tilesets = m_document->tilesets();

  m_previous = tilesets->at(m_id).name();
  tilesets->rename(m_id, m_name);

  emit m_document->renamed_tileset(m_id, m_name);
}

}  // namespace tactile::cmd
