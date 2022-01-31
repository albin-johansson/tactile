#include "preference_state.hpp"

#include <fstream>  // ifstream, ofstream
#include <ios>      // ios
#include <utility>  // move

#include <magic_enum.hpp>

#include "logging.hpp"
#include "settings.pb.h"

namespace tactile {
namespace {

constexpr Theme _def_theme = Theme::nocturnal;
constexpr cen::color _def_viewport_bg{60, 60, 60};

constexpr c_str _def_preferred_format = "YAML";

constexpr usize _def_command_capacity = 100;
constexpr int32 _def_preferred_tile_width = 32;
constexpr int32 _def_preferred_tile_height = 32;
constexpr int32 _def_viewport_overlay_pos = cen::to_underlying(overlay_pos::bottom_left);

constexpr uint64 _bit_embed_tilesets = 1u << 0u;
constexpr uint64 _bit_indent_output = 1u << 1u;
constexpr uint64 _bit_fold_tile_data = 1u << 2u;
constexpr uint64 _bit_show_grid = 1u << 3u;
constexpr uint64 _bit_show_layer_dock = 1u << 4u;
constexpr uint64 _bit_show_tileset_dock = 1u << 5u;
constexpr uint64 _bit_show_properties_dock = 1u << 6u;
constexpr uint64 _bit_show_log_dock = 1u << 7u;
constexpr uint64 _bit_window_border = 1u << 8u;
constexpr uint64 _bit_restore_layout = 1u << 9u;
constexpr uint64 _bit_restore_last_session = 1u << 10u;
constexpr uint64 _bit_show_component_dock = 1u << 11u;

constexpr uint64 _def_flags = _bit_show_grid | _bit_indent_output | _bit_show_layer_dock |
                              _bit_show_tileset_dock | _bit_show_properties_dock |
                              _bit_show_component_dock | _bit_restore_layout |
                              _bit_restore_last_session;

}  // namespace

#define PRINT_FLAG(Name, Mask) \
  log_info(Name "... {}", (mData->flags & (Mask)) ? "yes" : "no")

struct preference_state::preferences_data
{
  std::string preferred_format{_def_preferred_format};

  Theme theme{_def_theme};
  cen::color viewport_background{_def_viewport_bg};

  usize command_capacity{_def_command_capacity};
  int32 preferred_tile_width{_def_preferred_tile_width};
  int32 preferred_tile_height{_def_preferred_tile_height};
  int32 viewport_overlay_pos{_def_viewport_overlay_pos};

  uint64 flags{_def_flags};
};

preference_state::preference_state() : mData{std::make_unique<preferences_data>()} {}

preference_state::preference_state(const preference_state& other)
    : mData{std::make_unique<preferences_data>()}
{
  *mData = *other.mData;
}

preference_state::preference_state(preference_state&&) noexcept = default;

auto preference_state::operator=(const preference_state& other) -> preference_state&
{
  if (this != &other) {
    mData = std::make_unique<preferences_data>();
    *mData = *other.mData;
  }
  return *this;
}

auto preference_state::operator=(preference_state&&) noexcept
    -> preference_state& = default;

preference_state::~preference_state() noexcept = default;

void preference_state::print()
{
  log_info("Theme... {}", magic_enum::enum_name(mData->theme));
  log_info("Viewport background... {}", mData->viewport_background.as_rgb());

  log_info("Command capacity... {}", mData->command_capacity);
  log_info("Preferred tile width... {}", mData->preferred_tile_width);
  log_info("Preferred tile height... {}", mData->preferred_tile_height);

  log_info("Preferred format... {}", mData->preferred_format);
  log_info("Viewport overlay pos... {}", mData->viewport_overlay_pos);

  PRINT_FLAG("Embed tilesets", _bit_embed_tilesets);
  PRINT_FLAG("Indent output", _bit_indent_output);
  PRINT_FLAG("Fold tile data", _bit_fold_tile_data);

  PRINT_FLAG("Show grid", _bit_show_grid);
  PRINT_FLAG("Show layer dock", _bit_show_layer_dock);
  PRINT_FLAG("Show log dock", _bit_show_log_dock);
  PRINT_FLAG("Show tileset dock", _bit_show_tileset_dock);
  PRINT_FLAG("Show properties dock", _bit_show_properties_dock);
  PRINT_FLAG("Show component dock", _bit_show_component_dock);

  PRINT_FLAG("Window border", _bit_window_border);
  PRINT_FLAG("Restore layout", _bit_restore_layout);
  PRINT_FLAG("Restore last session", _bit_restore_last_session);
}

void preference_state::parse(const std::filesystem::path& path)
{
  std::ifstream stream{path, std::ios::in | std::ios::binary};

  proto::settings cfg;
  if (cfg.ParseFromIstream(&stream)) {
    if (cfg.has_theme()) {
      mData->theme = static_cast<Theme>(cfg.theme());
    }

    if (cfg.has_viewport_background()) {
      const auto& color = cfg.viewport_background();
      mData->viewport_background.set_red(static_cast<uint8>(color.red()));
      mData->viewport_background.set_green(static_cast<uint8>(color.green()));
      mData->viewport_background.set_blue(static_cast<uint8>(color.blue()));
      mData->viewport_background.set_alpha(static_cast<uint8>(color.alpha()));
    }

    if (cfg.has_show_grid()) {
      set_flag(_bit_show_grid, cfg.show_grid());
    }

    if (cfg.has_window_border()) {
      set_flag(_bit_window_border, cfg.window_border());
    }

    if (cfg.has_command_capacity()) {
      mData->command_capacity = cfg.command_capacity();
    }

    if (cfg.has_restore_last_session()) {
      set_flag(_bit_restore_last_session, cfg.restore_last_session());
    }

    if (cfg.has_preferred_tile_width()) {
      mData->preferred_tile_width = cfg.preferred_tile_width();
    }

    if (cfg.has_preferred_tile_height()) {
      mData->preferred_tile_height = cfg.preferred_tile_height();
    }

    if (cfg.has_preferred_format()) {
      mData->preferred_format = cfg.preferred_format();
    }

    if (cfg.has_embed_tilesets()) {
      set_flag(_bit_embed_tilesets, cfg.embed_tilesets());
    }

    if (cfg.has_indent_output()) {
      set_flag(_bit_indent_output, cfg.indent_output());
    }

    if (cfg.has_fold_tile_data()) {
      set_flag(_bit_fold_tile_data, cfg.fold_tile_data());
    }

    if (cfg.has_show_tileset_dock()) {
      set_flag(_bit_show_tileset_dock, cfg.show_tileset_dock());
    }

    if (cfg.has_show_layer_dock()) {
      set_flag(_bit_show_layer_dock, cfg.show_layer_dock());
    }

    if (cfg.has_show_properties_dock()) {
      set_flag(_bit_show_properties_dock, cfg.show_properties_dock());
    }

    if (cfg.has_show_log_dock()) {
      set_flag(_bit_show_log_dock, cfg.show_log_dock());
    }

    if (cfg.has_show_component_dock()) {
      set_flag(_bit_show_component_dock, cfg.show_component_dock());
    }

    if (cfg.has_restore_layout()) {
      set_flag(_bit_restore_layout, cfg.restore_layout());
    }

    if (cfg.has_viewport_overlay_pos()) {
      mData->viewport_overlay_pos = cfg.viewport_overlay_pos();
    }
  }
}

void preference_state::save(const std::filesystem::path& path)
{
  proto::settings cfg;

  cfg.set_theme(static_cast<proto::theme>(mData->theme));
  cfg.set_show_grid(is_grid_visible());
  cfg.set_window_border(has_window_border());

  {
    const auto& bg = viewport_bg();
    auto* background = cfg.mutable_viewport_background();
    background->set_red(bg.red());
    background->set_green(bg.green());
    background->set_blue(bg.blue());
    background->set_alpha(bg.alpha());
  }

  cfg.set_command_capacity(command_capacity());
  cfg.set_restore_last_session(will_restore_last_session());
  cfg.set_preferred_tile_width(preferred_tile_width());
  cfg.set_preferred_tile_height(preferred_tile_height());

  cfg.set_preferred_format(preferred_format());
  cfg.set_embed_tilesets(embed_tilesets());
  cfg.set_indent_output(indent_output());
  cfg.set_fold_tile_data(fold_tile_data());

  cfg.set_show_tileset_dock(is_tileset_dock_visible());
  cfg.set_show_layer_dock(is_layer_dock_visible());
  cfg.set_show_properties_dock(is_properties_dock_visible());
  cfg.set_show_log_dock(is_log_dock_visible());
  cfg.set_show_component_dock(is_component_dock_visible());
  cfg.set_restore_layout(will_restore_layout());
  cfg.set_viewport_overlay_pos(proto::overlay_pos{viewport_overlay_pos()});

  std::ofstream stream{path, std::ios::out | std::ios::trunc | std::ios::binary};
  if (!cfg.SerializeToOstream(&stream)) {
    log_error("Failed to save preferences!");
  }
}

void preference_state::reset_appearance_preferences()
{
  mData->theme = _def_theme;
  mData->viewport_background = _def_viewport_bg;

  reset_flag(_bit_window_border);
  reset_flag(_bit_show_grid);

  /* Note, not technically an appearance setting, but it is to the user */
  reset_flag(_bit_restore_layout);
}

void preference_state::reset_behavior_preferences()
{
  mData->command_capacity = _def_command_capacity;
  mData->preferred_tile_width = _def_preferred_tile_width;
  mData->preferred_tile_height = _def_preferred_tile_height;
  reset_flag(_bit_restore_last_session);
}

void preference_state::reset_export_preferences()
{
  mData->preferred_format = _def_preferred_format;
  reset_flag(_bit_embed_tilesets);
  reset_flag(_bit_indent_output);
  reset_flag(_bit_fold_tile_data);
}

void preference_state::reset_dock_visibilities()
{
  reset_flag(_bit_show_layer_dock);
  reset_flag(_bit_show_tileset_dock);
  reset_flag(_bit_show_properties_dock);
  reset_flag(_bit_show_component_dock);
  reset_flag(_bit_show_log_dock);
}

void preference_state::set_theme(const Theme theme)
{
  mData->theme = theme;
}

auto preference_state::get_theme() const -> Theme
{
  return mData->theme;
}

void preference_state::set_viewport_bg(const cen::color& bg)
{
  mData->viewport_background = bg;
}

auto preference_state::viewport_bg() const -> const cen::color&
{
  return mData->viewport_background;
}

void preference_state::set_window_border(const bool enable)
{
  set_flag(_bit_window_border, enable);
}

auto preference_state::has_window_border() const -> bool
{
  return test_flag(_bit_window_border);
}

void preference_state::set_layer_dock_visible(const bool visible)
{
  set_flag(_bit_show_layer_dock, visible);
}

auto preference_state::is_layer_dock_visible() const -> bool
{
  return test_flag(_bit_show_layer_dock);
}

void preference_state::set_tileset_dock_visible(const bool visible)
{
  set_flag(_bit_show_tileset_dock, visible);
}

auto preference_state::is_tileset_dock_visible() const -> bool
{
  return test_flag(_bit_show_tileset_dock);
}

void preference_state::set_properties_dock_visible(const bool visible)
{
  set_flag(_bit_show_properties_dock, visible);
}

auto preference_state::is_properties_dock_visible() const -> bool
{
  return test_flag(_bit_show_properties_dock);
}

void preference_state::set_component_dock_visible(const bool visible)
{
  set_flag(_bit_show_component_dock, visible);
}

auto preference_state::is_component_dock_visible() const -> bool
{
  return test_flag(_bit_show_component_dock);
}

void preference_state::set_log_dock_visible(const bool visible)
{
  set_flag(_bit_show_log_dock, visible);
}

auto preference_state::is_log_dock_visible() const -> bool
{
  return test_flag(_bit_show_log_dock);
}

void preference_state::set_grid_visible(const bool visible)
{
  set_flag(_bit_show_grid, visible);
}

auto preference_state::is_grid_visible() const -> bool
{
  return test_flag(_bit_show_grid);
}

void preference_state::set_embed_tilesets(const bool embed)
{
  set_flag(_bit_embed_tilesets, embed);
}

auto preference_state::embed_tilesets() const -> bool
{
  return test_flag(_bit_embed_tilesets);
}

void preference_state::set_fold_tile_data(const bool fold)
{
  set_flag(_bit_fold_tile_data, fold);
}

auto preference_state::fold_tile_data() const -> bool
{
  return test_flag(_bit_fold_tile_data);
}

void preference_state::set_indent_output(const bool indent)
{
  set_flag(_bit_indent_output, indent);
}

auto preference_state::indent_output() const -> bool
{
  return test_flag(_bit_indent_output);
}

void preference_state::set_will_restore_layout(const bool restore)
{
  set_flag(_bit_restore_layout, restore);
}

auto preference_state::will_restore_layout() const -> bool
{
  return test_flag(_bit_restore_layout);
}

void preference_state::set_will_restore_last_session(const bool restore)
{
  set_flag(_bit_restore_last_session, restore);
}

auto preference_state::will_restore_last_session() const -> bool
{
  return test_flag(_bit_restore_last_session);
}

void preference_state::set_viewport_overlay_pos(const overlay_pos pos)
{
  mData->viewport_overlay_pos = cen::to_underlying(pos);
}

auto preference_state::viewport_overlay_pos() const -> overlay_pos
{
  return static_cast<overlay_pos>(mData->viewport_overlay_pos);
}

void preference_state::set_command_capacity(const usize capacity)
{
  mData->command_capacity = capacity;
}

auto preference_state::command_capacity() const -> usize
{
  return mData->command_capacity;
}

void preference_state::set_preferred_format(std::string format)
{
  mData->preferred_format = std::move(format);
}

auto preference_state::preferred_format() const -> const std::string&
{
  return mData->preferred_format;
}

void preference_state::set_preferred_tile_width(const int32 width)
{
  mData->preferred_tile_width = width;
}

auto preference_state::preferred_tile_width() const -> int
{
  return mData->preferred_tile_width;
}

void preference_state::set_preferred_tile_height(const int32 height)
{
  mData->preferred_tile_height = height;
}

auto preference_state::preferred_tile_height() const -> int
{
  return mData->preferred_tile_height;
}

void preference_state::set_flag(const uint64 flag, const bool value) noexcept
{
  if (value) {
    mData->flags |= flag;
  }
  else {
    mData->flags &= ~flag;
  }
}

void preference_state::reset_flag(const uint64 flag) noexcept
{
  set_flag(flag, _def_flags & flag);
}

auto preference_state::test_flag(const uint64 flag) const noexcept -> bool
{
  return mData->flags & flag;
}

}  // namespace tactile