#include "duplicate_layer.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

DuplicateLayer::DuplicateLayer(not_null<core::MapDocument*> document,
                               const layer_id id)
    : QUndoCommand{QTranslator::tr("Duplicate Layer")}
    , mDocument{document}
    , mId{id}
{
  if (!mDocument)
  {
    throw tactile_error{"Null map document to duplicate layer command!"};
  }
}

void DuplicateLayer::undo()
{
  QUndoCommand::undo();

  const auto id = mNewId.value();
  mDocument->TakeLayer(id);

  emit mDocument->S_RemovedLayer(id);
  emit mDocument->S_Redraw();

  // We need to tell the document that it can safely reuse the layer ID
  mDocument->SetNextLayerId(id);
  mNewId.reset();
}

void DuplicateLayer::redo()
{
  QUndoCommand::redo();

  auto& [id, layer] = mDocument->Raw().DuplicateLayer(mId);
  layer->SetName(layer->Name() + QTranslator::tr(" (Copy)"));

  emit mDocument->S_AddedDuplicatedLayer(id, *mDocument->GetLayer(id));
  emit mDocument->S_Redraw();

  mNewId = id;
}

}  // namespace tactile::cmd
