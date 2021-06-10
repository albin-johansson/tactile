#include "set_layer_visibility.hpp"

#include <QTranslator>  // tr

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

SetLayerVisibility::SetLayerVisibility(not_null<core::MapDocument*> document,
                                       const layer_id id,
                                       const bool visible)
    : QUndoCommand{QTranslator::tr("Set Layer Visibility")}
    , mDocument{document}
    , mVisible{visible}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void SetLayerVisibility::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->Raw();
  map.SetVisibility(mId, !mVisible);

  emit mDocument->S_ChangedLayerVisibility(mId, !mVisible);
  emit mDocument->S_Redraw();
}

void SetLayerVisibility::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->Raw();
  map.SetVisibility(mId, mVisible);

  emit mDocument->S_ChangedLayerVisibility(mId, mVisible);
  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
