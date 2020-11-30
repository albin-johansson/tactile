#include "icons.hpp"

namespace tactile::gui::icons {

auto add() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/add.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto remove() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/remove.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto rename() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/rename.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto duplicate() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/new-copy.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto layer() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/layer.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto visible() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/visible.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto move_up() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/up.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto move_down() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/down.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}


auto expand() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/expand.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto collapse() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/collapse.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

}  // namespace tactile::gui::icons
