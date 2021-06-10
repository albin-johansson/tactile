#include "remove_layer.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

RemoveLayer::RemoveLayer(not_null<core::map_document*> document,
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

  auto& map = mDocument->raw();

  map.add_layer(mId, mLayer);
  while (map.index_of(mId).value() != mIndex)
  {
    if (map.index_of(mId).value() < mIndex)
    {
      map.move_layer_back(mId);
    }
    else
    {
      map.move_layer_forward(mId);
    }
  }

  emit mDocument->added_layer(mId, *mLayer);
  emit mDocument->redraw();
}

void RemoveLayer::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->raw();

  mIndex = map.index_of(mId).value();
  mLayer = map.take_layer(mId);

  emit mDocument->removed_layer(mId);
  emit mDocument->redraw();
}

}  // namespace tactile::cmd
