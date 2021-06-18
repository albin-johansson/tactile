#include "move_layer_back.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

MoveLayerBack::MoveLayerBack(NotNull<core::MapDocument*> document,
                             const layer_id id)
    : QUndoCommand{QTranslator::tr("Move Layer Back")}
    , mDocument{document}
    , mId{id}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void MoveLayerBack::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->Raw();
  map.MoveLayerForward(mId);

  emit mDocument->S_MovedLayerForward(mId);
  emit mDocument->S_Redraw();
}

void MoveLayerBack::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->Raw();
  map.MoveLayerBack(mId);

  emit mDocument->S_MovedLayerBack(mId);
  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
