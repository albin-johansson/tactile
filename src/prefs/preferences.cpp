#include "preferences.hpp"

#include <QString>

#include "theme.hpp"

namespace tactile::prefs {

auto window::last_layout_geometry() -> setting<QByteArray>
{
  static const auto str = QStringLiteral(u"window/lastLayoutGeometry");
  return setting<QByteArray>{str};
}

auto window::last_layout_state() -> setting<QByteArray>
{
  static const auto str = QStringLiteral(u"window/lastLayoutState");
  return setting<QByteArray>{str};
}

auto graphics::render_grid() -> setting<bool>
{
  static const auto str = QStringLiteral(u"graphics/grid");
  return setting<bool>{str};
}

auto graphics::theme() -> setting<QPalette>
{
  static const auto str = QStringLiteral(u"graphics/theme");
  return setting<QPalette>{str};
}

auto graphics::theme_name() -> setting<QString>
{
  static const auto str = QStringLiteral(u"graphics/themeName");
  return setting<QString>{str};
}

void validate()
{
  graphics::render_grid().set_if_missing(true);
  graphics::theme().set_if_missing(prefs::theme::get_default());
  graphics::theme_name().set_if_missing(theme::get_default_name().toString());
}

}  // namespace tactile::prefs
