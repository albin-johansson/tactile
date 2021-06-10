#include "select_layer.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

SelectLayer::SelectLayer(not_null<core::MapDocument*> document,
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
    auto& map = mDocument->Raw();
    map.SelectLayer(*mPrevious);

    const auto& layer = map.GetLayer(mId);
    Q_ASSERT(layer);

    emit mDocument->S_SelectedLayer(*mPrevious, *layer);

    mPrevious.reset();
  }
}

void SelectLayer::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->Raw();

  mPrevious = map.ActiveLayerId();
  map.SelectLayer(mId);

  const auto& layer = map.GetLayer(mId);
  Q_ASSERT(layer);

  emit mDocument->S_SelectedLayer(mId, *layer);
}

}  // namespace tactile::cmd
