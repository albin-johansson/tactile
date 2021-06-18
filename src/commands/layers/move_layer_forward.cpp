#include "move_layer_forward.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

MoveLayerForward::MoveLayerForward(NotNull<core::MapDocument*> document,
                                   const layer_id id)
    : QUndoCommand{QTranslator::tr("Move Layer Up")}
    , mDocument{document}
    , mId{id}
{
  if (!mDocument)
  {
    throw TactileError{"Null map document!"};
  }
}

void MoveLayerForward::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->Raw();
  map.MoveLayerBack(mId);

  emit mDocument->S_MovedLayerBack(mId);
  emit mDocument->S_Redraw();
}

void MoveLayerForward::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->Raw();
  map.MoveLayerForward(mId);

  emit mDocument->S_MovedLayerForward(mId);
  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
