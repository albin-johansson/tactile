#include "add_layer.hpp"

#include <utility>  // move

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

AddLayer::AddLayer(core::MapDocument* document,
                   shared<core::ILayer> layer,
                   const layer_id id)
    : QUndoCommand{TACTILE_QSTRING(u"Add Layer")}
    , mDocument{document}
    , mLayer{std::move(layer)}
    , mId{id}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create command from null map document!"};
  }

  if (!mLayer)
  {
    throw tactile_error{"Cannot create command from null tile_layer!"};
  }
}

void AddLayer::undo()
{
  QUndoCommand::undo();

  // We already have a shared pointer to the layer in question
  auto layer [[maybe_unused]] = mDocument->Raw().TakeLayer(mId);

  emit mDocument->S_RemovedLayer(mId);
  emit mDocument->S_Redraw();
}

void AddLayer::redo()
{
  QUndoCommand::redo();

  mDocument->Raw().AddLayer(mId, mLayer);

  emit mDocument->S_AddedLayer(mId, *mLayer);
  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
