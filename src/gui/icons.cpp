#include "icons.hpp"

#include "tactile_qstring.hpp"

namespace tactile {

auto TactileLogo() -> const QIcon&
{
  static const QIcon icon{TACTILE_QSTRING(u":resources/icons/icon.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconAdd() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/add.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconRemove() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/remove.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconRename() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/rename.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconDuplicate() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/new-copy.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconCopy() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/copy.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconOpen() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/opened-folder.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconSaveAs() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/save-as.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconReset() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/reset.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconPaste() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/paste.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconTileLayer() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/grid.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconObjectLayer() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/object.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconVisible() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/visible.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconInvisible() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/invisible.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconMoveUp() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/arrow-up.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconMoveDown() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/arrow-down.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconExpanded() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/expanded.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconCollapsed() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/collapsed.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto IconQuestionMark() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/question-mark.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

}  // namespace tactile
