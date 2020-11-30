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

auto graphics::theme_name_def() -> const QString&
{
  static const auto name = theme::get_default_name().toString();
  return name;
}

auto graphics::tool_widget_visible() -> setting<bool>
{
  static const auto str = QStringLiteral(u"graphics/toolWidgetVisible");
  return setting<bool>{str};
}

auto graphics::layer_widget_visible() -> setting<bool>
{
  static const auto str = QStringLiteral(u"graphics/layerWidgetVisible");
  return setting<bool>{str};
}

auto graphics::tileset_widget_visible() -> setting<bool>
{
  static const auto str = QStringLiteral(u"graphics/tilesetWidgetVisible");
  return setting<bool>{str};
}

auto graphics::properties_widget_visible() -> setting<bool>
{
  static const auto str = QStringLiteral(u"graphics/propertiesWidgetVisible");
  return setting<bool>{str};
}

void graphics::reset_tool_widget_visible()
{
  tool_widget_visible().set(tool_widget_visible_def());
}

void graphics::reset_layer_widget_visible()
{
  layer_widget_visible().set(layer_widget_visible_def());
}

void graphics::reset_tileset_widget_visible()
{
  tileset_widget_visible().set(tileset_widget_visible_def());
}

void graphics::reset_properties_widget_visible()
{
  properties_widget_visible().set(properties_widget_visible_def());
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

auto saves::default_format_def() -> const QString&
{
  static const auto format = QStringLiteral(u"JSON");
  return format;
}

void validate()
{
  {
    using namespace graphics;
    theme().set_if_missing(theme::get_default());
    theme_name().set_if_missing(theme::get_default_name().toString());
    render_grid().set_if_missing(render_grid_def());
    tool_widget_visible().set_if_missing(tool_widget_visible_def());
    tileset_widget_visible().set_if_missing(tileset_widget_visible_def());
    layer_widget_visible().set_if_missing(layer_widget_visible_def());
    properties_widget_visible().set_if_missing(properties_widget_visible_def());
  }

  saves::embed_tilesets().set_if_missing(saves::embed_tilesets_def());
  saves::generate_defaults().set_if_missing(saves::generate_defaults_def());
  saves::readable_output().set_if_missing(saves::readable_output_def());
  saves::tile_width().set_if_missing(saves::tile_width_def());
  saves::tile_height().set_if_missing(saves::tile_height_def());
  saves::default_format().set_if_missing(saves::default_format_def());
}

}  // namespace tactile::prefs
