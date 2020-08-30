#include "preferences.hpp"

#include <QString>

#include "theme.hpp"

namespace tactile::prefs {

auto window::last_layout_geometry() -> setting<QByteArray>&
{
  static const auto str = QStringLiteral(u"window/lastLayoutGeometry");
  static auto pref = setting<QByteArray>{str};
  return pref;
}

auto window::last_layout_state() -> setting<QByteArray>&
{
  static const auto str = QStringLiteral(u"window/lastLayoutState");
  static auto pref = setting<QByteArray>{str};
  return pref;
}

auto graphics::render_grid() -> setting<bool>&
{
  static const auto str = QStringLiteral(u"graphics/grid");
  static auto pref = setting<bool>{str};
  return pref;
}

auto graphics::theme() -> setting<QPalette>&
{
  static const auto str = QStringLiteral(u"graphics/theme");
  static auto pref = setting<QPalette>{str};
  return pref;
}

auto graphics::theme_name() -> setting<QString>&
{
  static const auto str = QStringLiteral(u"graphics/themeName");
  static auto pref = setting<QString>{str};
  return pref;
}

void validate()
{
  graphics::render_grid().set_if_missing(true);
  graphics::theme().set_if_missing(prefs::theme::get_default());
  graphics::theme_name().set_if_missing(theme::get_default_name().toString());
}

}  // namespace tactile::prefs
