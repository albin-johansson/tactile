#include "duplicate_layer.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

DuplicateLayer::DuplicateLayer(not_null<core::map_document*> document,
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
  mDocument->take_layer(id);

  emit mDocument->removed_layer(id);
  emit mDocument->redraw();

  // We need to tell the document that it can safely reuse the layer ID
  mDocument->set_next_layer_id(id);
  mNewId.reset();
}

void DuplicateLayer::redo()
{
  QUndoCommand::redo();

  auto& [id, layer] = mDocument->raw().duplicate_layer(mId);
  layer->SetName(layer->Name() + QTranslator::tr(" (Copy)"));

  emit mDocument->added_duplicated_layer(id, *mDocument->get_layer(id));
  emit mDocument->redraw();

  mNewId = id;
}

}  // namespace tactile::cmd
