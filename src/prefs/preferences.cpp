#include "preferences.hpp"

#include <qstring.h>

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

auto saves::embed_tilesets() -> setting<bool>
{
  static const auto str = QStringLiteral(u"saves/embedTilesets");
  return setting<bool>{str};
}

auto saves::generate_defaults() -> setting<bool>
{
  static const auto str = QStringLiteral(u"saves/generateDefaults");
  return setting<bool>{str};
}

auto saves::tile_width() -> setting<int>
{
  static const auto str = QStringLiteral(u"saves/tileWidth");
  return setting<int>{str};
}

auto saves::tile_height() -> setting<int>
{
  static const auto str = QStringLiteral(u"saves/tileHeight");
  return setting<int>{str};
}

void validate()
{
  graphics::render_grid().set_if_missing(true);
  graphics::theme().set_if_missing(theme::get_default());
  graphics::theme_name().set_if_missing(theme::get_default_name().toString());

  saves::embed_tilesets().set_if_missing(true);
  saves::generate_defaults().set_if_missing(false);
  saves::tile_width().set_if_missing(32);
  saves::tile_height().set_if_missing(32);
}

}  // namespace tactile::prefs
