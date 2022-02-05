#pragma once

#include <filesystem>   // path
#include <memory>       // unique_ptr
#include <string>       // string
#include <string_view>  // string_view

#include <centurion.hpp>

#include "editor/gui/themes.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Encodes different viewport overlay positions.
 *
 * \warning Do not change the enumerator values!
 *
 * \see proto::overlay_pos
 */
enum class overlay_pos {
  top_left = 0,
  top_right = 1,
  bottom_left = 2,
  bottom_right = 3
};

class preference_state final {
 public:
  preference_state();

  preference_state(const preference_state& other);

  preference_state(preference_state&& other) noexcept;

  auto operator=(const preference_state& other) -> preference_state&;

  auto operator=(preference_state&& other) noexcept -> preference_state&;

  ~preference_state() noexcept;

  void parse(const std::filesystem::path& path);

  void save(const std::filesystem::path& path);

  void print();

  void reset_appearance_preferences();

  void reset_behavior_preferences();

  void reset_export_preferences();

  void reset_dock_visibilities();

  void set_theme(editor_theme theme);
  [[nodiscard]] auto get_theme() const -> editor_theme;

  void set_viewport_bg(const cen::color& bg);
  [[nodiscard]] auto viewport_bg() const -> const cen::color&;

  void set_window_border(bool enable);
  [[nodiscard]] auto has_window_border() const -> bool;

  void set_layer_dock_visible(bool visible);
  [[nodiscard]] auto is_layer_dock_visible() const -> bool;

  void set_tileset_dock_visible(bool visible);
  [[nodiscard]] auto is_tileset_dock_visible() const -> bool;

  void set_properties_dock_visible(bool visible);
  [[nodiscard]] auto is_properties_dock_visible() const -> bool;

  void set_component_dock_visible(bool visible);
  [[nodiscard]] auto is_component_dock_visible() const -> bool;

  void set_log_dock_visible(bool visible);
  [[nodiscard]] auto is_log_dock_visible() const -> bool;

  void set_grid_visible(bool visible);
  [[nodiscard]] auto is_grid_visible() const -> bool;

  void set_embed_tilesets(bool embed);
  [[nodiscard]] auto embed_tilesets() const -> bool;

  void set_fold_tile_data(bool fold);
  [[nodiscard]] auto fold_tile_data() const -> bool;

  void set_indent_output(bool indent);
  [[nodiscard]] auto indent_output() const -> bool;

  void set_will_restore_layout(bool restore);
  [[nodiscard]] auto will_restore_layout() const -> bool;

  void set_will_restore_last_session(bool restore);
  [[nodiscard]] auto will_restore_last_session() const -> bool;

  void set_viewport_overlay_pos(overlay_pos pos);
  [[nodiscard]] auto viewport_overlay_pos() const -> overlay_pos;

  void set_command_capacity(usize capacity);
  [[nodiscard]] auto command_capacity() const -> usize;

  void set_preferred_format(std::string format);
  [[nodiscard]] auto preferred_format() const -> const std::string&;

  void set_preferred_tile_width(int32 width);
  [[nodiscard]] auto preferred_tile_width() const -> int;

  void set_preferred_tile_height(int32 height);
  [[nodiscard]] auto preferred_tile_height() const -> int;

 private:
  struct preferences_data;
  std::unique_ptr<preferences_data> mData;

  void set_flag(uint64 flag, bool value) noexcept;

  void reset_flag(uint64 flag) noexcept;

  [[nodiscard]] auto test_flag(uint64 flag) const noexcept -> bool;
};

}  // namespace tactile
