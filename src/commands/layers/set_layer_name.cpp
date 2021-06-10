#include "set_layer_name.hpp"

#include <QTranslator>  // tr
#include <utility>      // move

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

SetLayerName::SetLayerName(not_null<core::map_document*> document,
                           const layer_id id,
                           QString name)
    : QUndoCommand{QTranslator::tr("Set Layer Name")}
    , mDocument{document}
    , mId{id}
    , mName{std::move(name)}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void SetLayerName::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->raw();
  map.set_name(mId, mPrevious.value());

  emit mDocument->changed_layer_name(mId, mPrevious.value());
  emit mDocument->redraw();

  mPrevious.reset();
}

void SetLayerName::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->raw();

  mPrevious = map.name(mId);
  map.set_name(mId, mName);

  emit mDocument->changed_layer_name(mId, mName);
  emit mDocument->redraw();
}

}  // namespace tactile::cmd
