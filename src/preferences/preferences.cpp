#include "preferences.hpp"

#include "tactile_qstring.hpp"

namespace tactile::prefs {

auto LastLayoutGeometry() -> Setting<QByteArray>
{
  static const auto str = TACTILE_QSTRING(u"window/lastLayoutGeometry");
  return Setting<QByteArray>{str};
}

auto LastLayoutState() -> Setting<QByteArray>
{
  static const auto str = TACTILE_QSTRING(u"window/lastLayoutState");
  return Setting<QByteArray>{str};
}

auto RenderGrid() -> Setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/grid");
  return Setting<bool>{str};
}

auto Theme() -> Setting<QPalette>
{
  static const auto str = TACTILE_QSTRING(u"graphics/theme");
  return Setting<QPalette>{str};
}

auto UserThemes() -> Setting<theme_map>
{
  static const auto str = TACTILE_QSTRING(u"graphics/userThemes");
  return Setting<theme_map>{str};
}

auto UserThemesDefault() -> theme_map
{
  return theme_map{};
}

auto ThemeName() -> Setting<QString>
{
  static const auto str = TACTILE_QSTRING(u"graphics/themeName");
  return Setting<QString>{str};
}

auto AccentColor() -> Setting<QColor>
{
  static const auto str = TACTILE_QSTRING(u"graphics/accentColor");
  return Setting<QColor>{str};
}

auto ToolWidgetVisibility() -> Setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/toolWidgetVisible");
  return Setting<bool>{str};
}

auto LayerWidgetVisibility() -> Setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/layerWidgetVisible");
  return Setting<bool>{str};
}

auto TilesetWidgetVisibility() -> Setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/tilesetWidgetVisible");
  return Setting<bool>{str};
}

auto PropertiesWidgetVisibility() -> Setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/propertiesWidgetVisible");
  return Setting<bool>{str};
}

auto UseOpenGl() -> Setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"graphics/useOpenGL");
  return Setting<bool>{str};
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

auto EmbedTilesets() -> Setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"saves/embedTilesets");
  return Setting<bool>{str};
}

auto GenerateDefaults() -> Setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"saves/generateDefaults");
  return Setting<bool>{str};
}

auto UseReadableOutput() -> Setting<bool>
{
  static const auto str = TACTILE_QSTRING(u"saves/humanReadableOutput");
  return Setting<bool>{str};
}

auto TileWidth() -> Setting<int>
{
  static const auto str = TACTILE_QSTRING(u"saves/tileWidth");
  return Setting<int>{str};
}

auto TileHeight() -> Setting<int>
{
  static const auto str = TACTILE_QSTRING(u"saves/tileHeight");
  return Setting<int>{str};
}

auto DefaultFormat() -> Setting<QString>
{
  static const auto str = TACTILE_QSTRING(u"saves/defaultFormat");
  return Setting<QString>{str};
}

auto DefaultFormatDefault() -> const QString&
{
  static const auto format = TACTILE_QSTRING(u"JSON");
  return format;
}

void ValidatePreferences()
{
  RenderGrid().SetIfMissing(render_grid_def);
  ToolWidgetVisibility().SetIfMissing(tool_widget_visible_def);
  TilesetWidgetVisibility().SetIfMissing(tileset_widget_visible_def);
  LayerWidgetVisibility().SetIfMissing(layer_widget_visible_def);
  PropertiesWidgetVisibility().SetIfMissing(properties_widget_visible_def);
  AccentColor().SetIfMissing(accent_color_def);
  UseOpenGl().SetIfMissing(use_opengl_def);

  // Note, the current theme is validated by the validate_themes-function
  UserThemes().SetIfMissing(UserThemesDefault());

  EmbedTilesets().SetIfMissing(embed_tilesets_def);
  GenerateDefaults().SetIfMissing(generate_defaults_def);
  UseReadableOutput().SetIfMissing(readable_output_def);
  DefaultFormat().SetIfMissing(DefaultFormatDefault());
  TileWidth().SetIfMissing(tile_width_def);
  TileHeight().SetIfMissing(tile_height_def);
}

}  // namespace tactile::prefs
