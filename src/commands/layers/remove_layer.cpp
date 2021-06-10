#include "remove_layer.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

RemoveLayer::RemoveLayer(not_null<core::MapDocument*> document,
                         const layer_id id)
    : QUndoCommand{TACTILE_QSTRING(u"Remove Layer")}
    , mDocument{document}
    , mId{id}
{
  if (!mDocument)
  {
    throw tactile_error{"Null map document!"};
  }
}

void RemoveLayer::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->Raw();

  map.AddLayer(mId, mLayer);
  while (map.IndexOf(mId).value() != mIndex)
  {
    if (map.IndexOf(mId).value() < mIndex)
    {
      map.MoveLayerBack(mId);
    }
    else
    {
      map.MoveLayerForward(mId);
    }
  }

  emit mDocument->S_AddedLayer(mId, *mLayer);
  emit mDocument->S_Redraw();
}

void RemoveLayer::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->Raw();

  mIndex = map.IndexOf(mId).value();
  mLayer = map.TakeLayer(mId);

  emit mDocument->S_RemovedLayer(mId);
  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
