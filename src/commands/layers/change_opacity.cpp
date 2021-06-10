#include "change_opacity.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

ChangeOpacity::ChangeOpacity(not_null<core::MapDocument*> document,
                             const layer_id id,
                             const double opacity)
    : QUndoCommand{QTranslator::tr("Change Layer Opacity")}
    , mDocument{document}
    , mId{id}
    , mOpacity{opacity}
{
  if (!mDocument)
  {
    throw tactile_error{"Null map document for change opacity command!"};
  }
}

void ChangeOpacity::undo()
{
  QUndoCommand::undo();

  const auto opacity = mPreviousOpacity.value();
  mDocument->Raw().SetOpacity(mId, opacity);

  emit mDocument->S_ChangedLayerOpacity(mId, opacity);
  emit mDocument->S_Redraw();

  mPreviousOpacity.reset();
}

void ChangeOpacity::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->Raw();

  mPreviousOpacity = map.GetLayer(mId)->Opacity();
  map.SetOpacity(mId, mOpacity);

  emit mDocument->S_ChangedLayerOpacity(mId, mOpacity);
  emit mDocument->S_Redraw();
}

auto ChangeOpacity::mergeWith(const QUndoCommand* other) -> bool
{
  if (id() == other->id())
  {
    if (auto* otherCommand = dynamic_cast<const ChangeOpacity*>(other))
    {
      mOpacity = otherCommand->mOpacity;
      return true;
    }
  }

  return false;
}

}  // namespace tactile::cmd
