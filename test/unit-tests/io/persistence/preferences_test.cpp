#include "io/persistence/preferences.hpp"

#include <gtest/gtest.h>

#include "core/common/enum.hpp"
#include "settings.pb.h"

using namespace tactile::io;
using namespace tactile::ui;

namespace tactile::test {

TEST(Preferences, Defaults)
{
  const PreferenceState state;

  ASSERT_EQ(def_preferred_format, state.preferred_format);
  ASSERT_EQ(def_theme, state.theme);
  ASSERT_EQ(def_viewport_bg, state.viewport_background);
  ASSERT_EQ(def_command_capacity, state.command_capacity);
  ASSERT_EQ(def_preferred_tile_size, state.preferred_tile_size);
  ASSERT_EQ(def_font_size, state.font_size);
  ASSERT_EQ(def_viewport_overlay_pos, state.viewport_overlay_pos);
  ASSERT_EQ(def_embed_tilesets, state.embed_tilesets);
  ASSERT_EQ(def_indent_output, state.indent_output);
  ASSERT_EQ(def_fold_tile_data, state.fold_tile_data);
  ASSERT_EQ(def_show_grid, state.show_grid);
  ASSERT_EQ(def_show_layer_dock, state.show_layer_dock);
  ASSERT_EQ(def_show_tileset_dock, state.show_tileset_dock);
  ASSERT_EQ(def_show_property_dock, state.show_property_dock);
  ASSERT_EQ(def_show_component_dock, state.show_component_dock);
  ASSERT_EQ(def_show_log_dock, state.show_log_dock);
  ASSERT_EQ(def_window_border, state.window_border);
  ASSERT_EQ(def_restore_layout, state.restore_layout);
  ASSERT_EQ(def_restore_last_session, state.restore_last_session);
  ASSERT_EQ(def_show_viewport_overlay_fps, state.show_viewport_overlay_fps);
  ASSERT_EQ(def_highlight_active_layer, state.highlight_active_layer);
  ASSERT_EQ(def_use_default_font, state.use_default_font);
}

TEST(Preferences, EnsureThemeEnumsMatch)
{
  ASSERT_EQ(proto::THEME_DEAR_DARK, to_underlying(EditorTheme::DearDark));
  ASSERT_EQ(proto::THEME_DEAR_LIGHT, to_underlying(EditorTheme::DearLight));
  ASSERT_EQ(proto::THEME_RUBY, to_underlying(EditorTheme::Ruby));
  ASSERT_EQ(proto::THEME_SAPPHIRE, to_underlying(EditorTheme::Sapphire));
  ASSERT_EQ(proto::THEME_EMERALD, to_underlying(EditorTheme::Emerald));
  ASSERT_EQ(proto::THEME_AMETHYST, to_underlying(EditorTheme::Amethyst));
  ASSERT_EQ(proto::THEME_AMBER, to_underlying(EditorTheme::Amber));
  ASSERT_EQ(proto::THEME_NOCTURNAL, to_underlying(EditorTheme::Nocturnal));
  ASSERT_EQ(proto::THEME_ASH, to_underlying(EditorTheme::Ash));
  ASSERT_EQ(proto::THEME_DIAMOND, to_underlying(EditorTheme::Diamond));
  ASSERT_EQ(proto::THEME_JOKER, to_underlying(EditorTheme::Joker));
  ASSERT_EQ(proto::THEME_RASPBERRY, to_underlying(EditorTheme::Raspberry));
}

TEST(Preferences, EnsureOverlayPosEnumsMatch)
{
  ASSERT_EQ(proto::OVERLAY_POS_TOP_LEFT, to_underlying(OverlayPos::TopLeft));
  ASSERT_EQ(proto::OVERLAY_POS_TOP_RIGHT, to_underlying(OverlayPos::TopRight));
  ASSERT_EQ(proto::OVERLAY_POS_BOTTOM_RIGHT, to_underlying(OverlayPos::BottomRight));
  ASSERT_EQ(proto::OVERLAY_POS_BOTTOM_LEFT, to_underlying(OverlayPos::BottomLeft));
}

}  // namespace tactile::test
