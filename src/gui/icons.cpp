#include "icons.hpp"

#include "tactile_qstring.hpp"

namespace tactile::gui::icons {

auto add() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/add.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto remove() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/remove.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto rename() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/rename.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto duplicate() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/new-copy.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto copy() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/copy.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto paste() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/paste.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto layer() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/grid.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto visible() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/visible.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto invisible() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/invisible.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto move_up() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/arrow-up.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto move_down() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/fluent/64/arrow-down.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto expanded() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/expanded.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto collapsed() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/collapsed.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto object() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/object.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto question_mark() -> const QIcon&
{
  static const QIcon icon{
      TACTILE_QSTRING(u":resources/icons/color/64/question-mark.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

}  // namespace tactile::gui::icons
