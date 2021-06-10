#include "select_layer.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

SelectLayer::SelectLayer(not_null<core::map_document*> document,
                         const layer_id id)
    : QUndoCommand{QTranslator::tr("Select Layer")}
    , mDocument{document}
    , mId{id}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void SelectLayer::undo()
{
  QUndoCommand::undo();

  if (mPrevious)
  {
    auto& map = mDocument->raw();
    map.select_layer(*mPrevious);

    const auto& layer = map.get_layer(mId);
    Q_ASSERT(layer);

    emit mDocument->selected_layer(*mPrevious, *layer);

    mPrevious.reset();
  }
}

void SelectLayer::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->raw();

  mPrevious = map.active_layer_id();
  map.select_layer(mId);

  const auto& layer = map.get_layer(mId);
  Q_ASSERT(layer);

  emit mDocument->selected_layer(mId, *layer);
}

}  // namespace tactile::cmd
