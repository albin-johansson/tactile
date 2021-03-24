#include "preferences.hpp"

#include "tactile_qstring.hpp"

namespace tactile::prefs {

auto window::last_layout_geometry() -> setting<QByteArray>
{
  static const auto str = TACTILE_QSTRING(u"window/lastLayoutGeometry");
  return setting<QByteArray>{str};
}

auto window::last_layout_state() -> setting<QByteArray>
{
  static const auto str = TACTILE_QSTRING(u"window/lastLayoutState");
  return setting<QByteArray>{str};
}

auto gfx::render_grid() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/grid");
  return setting<bool>{str};
}

auto gfx::theme() -> setting<QPalette>
{
  static const auto str = TACTILE_QSTRING(u"graphics/theme");
  return setting<QPalette>{str};
}

auto gfx::user_themes() -> setting<theme_map>
{
  static const auto str = TACTILE_QSTRING(u"graphics/userThemes");
  return setting<theme_map>{str};
}

auto gfx::user_themes_def() -> theme_map
{
  return theme_map{};
}

auto gfx::theme_name() -> setting<QString>
{
  static const auto str = TACTILE_QSTRING(u"graphics/themeName");
  return setting<QString>{str};
}

auto gfx::accent_color() -> setting<QColor>
{
  static const auto str = TACTILE_QSTRING(u"graphics/accentColor");
  return setting<QColor>{str};
}

// auto gfx::theme_name_def() -> const QString&
//{
//  static const auto name = get_default_name().toString();
//  return name;
//}

auto gfx::tool_widget_visible() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/toolWidgetVisible");
  return setting<bool>{str};
}

auto gfx::layer_widget_visible() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/layerWidgetVisible");
  return setting<bool>{str};
}

auto gfx::tileset_widget_visible() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/tilesetWidgetVisible");
  return setting<bool>{str};
}

auto gfx::properties_widget_visible() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/propertiesWidgetVisible");
  return setting<bool>{str};
}

auto gfx::use_opengl() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/useOpenGL");
  return setting<bool>{str};
}

void gfx::reset_tool_widget_visible()
{
  tool_widget_visible() = tool_widget_visible_def();
}

void gfx::reset_layer_widget_visible()
{
  layer_widget_visible() = layer_widget_visible_def();
}

void gfx::reset_tileset_widget_visible()
{
  tileset_widget_visible() = tileset_widget_visible_def();
}

void gfx::reset_properties_widget_visible()
{
  properties_widget_visible() = properties_widget_visible_def();
}

auto saves::embed_tilesets() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"saves/embedTilesets");
  return setting<bool>{str};
}

auto saves::generate_defaults() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"saves/generateDefaults");
  return setting<bool>{str};
}

auto saves::readable_output() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"saves/humanReadableOutput");
  return setting<bool>{str};
}

auto saves::tile_width() -> setting<int>
{
  static const auto str = TACTILE_QSTRING(u"saves/tileWidth");
  return setting<int>{str};
}

auto saves::tile_height() -> setting<int>
{
  static const auto str = TACTILE_QSTRING(u"saves/tileHeight");
  return setting<int>{str};
}

auto saves::default_format() -> setting<QString>
{
  static const auto str = TACTILE_QSTRING(u"saves/defaultFormat");
  return setting<QString>{str};
}

auto saves::default_format_def() -> const QString&
{
  static const auto format = TACTILE_QSTRING(u"JSON");
  return format;
}

void validate()
{
  // clang-format off
  gfx::render_grid().set_if_missing(gfx::render_grid_def());
  gfx::tool_widget_visible().set_if_missing(gfx::tool_widget_visible_def());
  gfx::tileset_widget_visible().set_if_missing(gfx::tileset_widget_visible_def());
  gfx::layer_widget_visible().set_if_missing(gfx::layer_widget_visible_def());
  gfx::properties_widget_visible().set_if_missing(gfx::properties_widget_visible_def());
  gfx::accent_color().set_if_missing(gfx::accent_color_def());
  gfx::use_opengl().set_if_missing(gfx::use_opengl_def());

  // Note, the current theme is validated by the validate_themes-function
  gfx::user_themes().set_if_missing(gfx::user_themes_def());
  // clang-format on

  saves::embed_tilesets().set_if_missing(saves::embed_tilesets_def());
  saves::generate_defaults().set_if_missing(saves::generate_defaults_def());
  saves::readable_output().set_if_missing(saves::readable_output_def());
  saves::tile_width().set_if_missing(saves::tile_width_def());
  saves::tile_height().set_if_missing(saves::tile_height_def());
  saves::default_format().set_if_missing(saves::default_format_def());
}

}  // namespace tactile::prefs
