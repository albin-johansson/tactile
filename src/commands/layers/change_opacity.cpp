#include "change_opacity.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

ChangeOpacity::ChangeOpacity(not_null<core::map_document*> document,
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
  mDocument->raw().set_opacity(mId, opacity);

  emit mDocument->changed_layer_opacity(mId, opacity);
  emit mDocument->redraw();

  mPreviousOpacity.reset();
}

void ChangeOpacity::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->raw();

  mPreviousOpacity = map.get_layer(mId)->Opacity();
  map.set_opacity(mId, mOpacity);

  emit mDocument->changed_layer_opacity(mId, mOpacity);
  emit mDocument->redraw();
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
