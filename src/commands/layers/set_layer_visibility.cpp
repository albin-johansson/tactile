#include "set_layer_visibility.hpp"

#include <QTranslator>  // tr

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

SetLayerVisibility::SetLayerVisibility(not_null<core::map_document*> document,
                                       const layer_id id,
                                       const bool visible)
    : QUndoCommand{QTranslator::tr("Set Layer Visibility")}
    , mDocument{document}
    , mVisible{visible}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void SetLayerVisibility::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->raw();
  map.set_visibility(mId, !mVisible);

  emit mDocument->changed_layer_visibility(mId, !mVisible);
  emit mDocument->redraw();
}

void SetLayerVisibility::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->raw();
  map.set_visibility(mId, mVisible);

  emit mDocument->changed_layer_visibility(mId, mVisible);
  emit mDocument->redraw();
}

}  // namespace tactile::cmd
