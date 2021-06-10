#include "move_layer_forward.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

MoveLayerForward::MoveLayerForward(not_null<core::map_document*> document,
                                   const layer_id id)
    : QUndoCommand{QTranslator::tr("Move Layer Up")}
    , mDocument{document}
    , mId{id}
{
  if (!mDocument)
  {
    throw tactile_error{"Null map document!"};
  }
}

void MoveLayerForward::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->raw();
  map.move_layer_back(mId);

  emit mDocument->moved_layer_back(mId);
  emit mDocument->redraw();
}

void MoveLayerForward::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->raw();
  map.move_layer_forward(mId);

  emit mDocument->moved_layer_forward(mId);
  emit mDocument->redraw();
}

}  // namespace tactile::cmd
