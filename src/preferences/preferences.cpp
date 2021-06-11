#include "preferences.hpp"

#include "tactile_qstring.hpp"

namespace tactile::prefs {

auto LastLayoutGeometry() -> setting<QByteArray>
{
  static const auto str = TACTILE_QSTRING(u"window/lastLayoutGeometry");
  return setting<QByteArray>{str};
}

auto LastLayoutState() -> setting<QByteArray>
{
  static const auto str = TACTILE_QSTRING(u"window/lastLayoutState");
  return setting<QByteArray>{str};
}

auto RenderGrid() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/grid");
  return setting<bool>{str};
}

auto Theme() -> setting<QPalette>
{
  static const auto str = TACTILE_QSTRING(u"graphics/theme");
  return setting<QPalette>{str};
}

auto UserThemes() -> setting<theme_map>
{
  static const auto str = TACTILE_QSTRING(u"graphics/userThemes");
  return setting<theme_map>{str};
}

auto UserThemesDefault() -> theme_map
{
  return theme_map{};
}

auto ThemeName() -> setting<QString>
{
  static const auto str = TACTILE_QSTRING(u"graphics/themeName");
  return setting<QString>{str};
}

auto AccentColor() -> setting<QColor>
{
  static const auto str = TACTILE_QSTRING(u"graphics/accentColor");
  return setting<QColor>{str};
}

auto ToolWidgetVisibility() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/toolWidgetVisible");
  return setting<bool>{str};
}

auto LayerWidgetVisibility() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/layerWidgetVisible");
  return setting<bool>{str};
}

auto TilesetWidgetVisibility() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/tilesetWidgetVisible");
  return setting<bool>{str};
}

auto PropertiesWidgetVisibility() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/propertiesWidgetVisible");
  return setting<bool>{str};
}

auto UseOpenGl() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/useOpenGL");
  return setting<bool>{str};
}

void ResetToolWidgetVisibility()
{
  ToolWidgetVisibility() = tool_widget_visible_def;
}

void ResetLayerWidgetVisibility()
{
  LayerWidgetVisibility() = layer_widget_visible_def;
}

void ResetTilesetWidgetVisibility()
{
  TilesetWidgetVisibility() = tileset_widget_visible_def;
}

void ResetPropertiesWidgetVisibility()
{
  PropertiesWidgetVisibility() = properties_widget_visible_def;
}

auto EmbedTilesets() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"saves/embedTilesets");
  return setting<bool>{str};
}

auto GenerateDefaults() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"saves/generateDefaults");
  return setting<bool>{str};
}

auto UseReadableOutput() -> setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"saves/humanReadableOutput");
  return setting<bool>{str};
}

auto TileWidth() -> setting<int>
{
  static const auto str = TACTILE_QSTRING(u"saves/tileWidth");
  return setting<int>{str};
}

auto TileHeight() -> setting<int>
{
  static const auto str = TACTILE_QSTRING(u"saves/tileHeight");
  return setting<int>{str};
}

auto DefaultFormat() -> setting<QString>
{
  static const auto str = TACTILE_QSTRING(u"saves/defaultFormat");
  return setting<QString>{str};
}

auto DefaultFormatDefault() -> const QString&
{
  static const auto format = TACTILE_QSTRING(u"JSON");
  return format;
}

void ValidatePreferences()
{
  RenderGrid().set_if_missing(render_grid_def);
  ToolWidgetVisibility().set_if_missing(tool_widget_visible_def);
  TilesetWidgetVisibility().set_if_missing(tileset_widget_visible_def);
  LayerWidgetVisibility().set_if_missing(layer_widget_visible_def);
  PropertiesWidgetVisibility().set_if_missing(properties_widget_visible_def);
  AccentColor().set_if_missing(accent_color_def);
  UseOpenGl().set_if_missing(use_opengl_def);

  // Note, the current theme is validated by the validate_themes-function
  UserThemes().set_if_missing(UserThemesDefault());

  EmbedTilesets().set_if_missing(embed_tilesets_def);
  GenerateDefaults().set_if_missing(generate_defaults_def);
  UseReadableOutput().set_if_missing(readable_output_def);
  DefaultFormat().set_if_missing(DefaultFormatDefault());
  TileWidth().set_if_missing(tile_width_def);
  TileHeight().set_if_missing(tile_height_def);
}

}  // namespace tactile::prefs
