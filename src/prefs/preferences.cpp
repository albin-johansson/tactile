#include "preferences.hpp"

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

auto saves::readable_output() -> setting<bool>
{
  static const auto str = QStringLiteral(u"saves/humanReadableOutput");
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

auto saves::default_format() -> setting<QString>
{
  static const auto str = QStringLiteral(u"saves/defaultFormat");
  return setting<QString>{str};
}

void validate()
{
  graphics::render_grid().set_if_missing(false);
  graphics::theme().set_if_missing(theme::get_default());
  graphics::theme_name().set_if_missing(theme::get_default_name().toString());

  saves::embed_tilesets().set_if_missing(false);
  saves::generate_defaults().set_if_missing(false);
  saves::readable_output().set_if_missing(true);
  saves::tile_width().set_if_missing(32);
  saves::tile_height().set_if_missing(32);
  saves::default_format().set_if_missing(QStringLiteral(u"JSON"));
}

}  // namespace tactile::prefs
