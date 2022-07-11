/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "settings_dialog.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/events/command_events.hpp"
#include "core/events/misc_events.hpp"
#include "editor/ui/common/buttons.hpp"
#include "editor/ui/common/checkboxes.hpp"
#include "editor/ui/common/colors.hpp"
#include "editor/ui/common/tooltips.hpp"
#include "editor/ui/fonts.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/themes.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile::ui {
namespace {

void _update_preview_settings(const io::PreferenceState& prefs)
{
  apply_theme(ImGui::GetStyle(), prefs.theme);
  ImGui::GetStyle().WindowBorderSize = prefs.window_border ? 1.0f : 0.0f;
}

void _reset_appearance_preferences(io::PreferenceState& prefs)
{
  prefs.theme = io::def_theme;
  prefs.viewport_background = io::def_viewport_bg;

  prefs.window_border = io::def_window_border;
  prefs.show_grid = io::def_show_grid;

  prefs.restore_layout = io::def_restore_layout;
  prefs.use_default_font = io::def_use_default_font;
  prefs.font_size = ui::def_font_size;
}

void _reset_behavior_preferences(io::PreferenceState& prefs)
{
  prefs.command_capacity = io::def_command_capacity;
  prefs.preferred_tile_size = io::def_preferred_tile_size;
  prefs.restore_last_session = io::def_restore_last_session;
}

void _reset_export_preferences(io::PreferenceState& prefs)
{
  prefs.preferred_format = io::def_preferred_format;
  prefs.embed_tilesets = io::def_embed_tilesets;
  prefs.indent_output = io::def_indent_output;
  prefs.fold_tile_data = io::def_fold_tile_data;
}

}  // namespace

SettingsDialog::SettingsDialog() : ADialog {"Settings"}
{
  use_apply_button();
}

void SettingsDialog::show()
{
  mSnapshot = io::get_preferences();
  mUiSettings = mSnapshot;
  make_visible();
}

void SettingsDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  if (TabBar bar {"##SettingsTabBar"}; bar.is_open()) {
    update_behavior_tab();
    update_appearance_tab();
    update_export_tab();
  }
}

void SettingsDialog::on_cancel()
{
  /* Reset any changes we made for preview purposes */
  _update_preview_settings(io::get_preferences());
}

void SettingsDialog::on_accept(entt::dispatcher& dispatcher)
{
  apply_settings(dispatcher);
  _update_preview_settings(io::get_preferences());
}

void SettingsDialog::on_apply(entt::dispatcher& dispatcher)
{
  apply_settings(dispatcher);
  _update_preview_settings(io::get_preferences());
}

void SettingsDialog::apply_settings(entt::dispatcher& dispatcher)
{
  io::set_preferences(mUiSettings);

  if (mUiSettings.command_capacity != mSnapshot.command_capacity) {
    dispatcher.enqueue<SetCommandCapacityEvent>(mUiSettings.command_capacity);
  }

  if (mUiSettings.use_default_font != mSnapshot.use_default_font ||
      mUiSettings.font_size != mSnapshot.font_size) {
    dispatcher.enqueue<ReloadFontsEvent>();
  }

  mSnapshot = mUiSettings;
}

void SettingsDialog::update_behavior_tab()
{
  if (TabItem item {"Behavior"}; item.is_open()) {
    ImGui::Spacing();
    if (button("Restore Defaults")) {
      _reset_behavior_preferences(mUiSettings);
      _update_preview_settings(mUiSettings);
    }
    ImGui::Spacing();

    checkbox("Restore last session on startup", &mUiSettings.restore_last_session);

    ImGui::DragInt("Preferred tile width",
                   &mUiSettings.preferred_tile_size.x,
                   1.0f,
                   1,
                   10'000);
    lazy_tooltip("##PreferredTileWidthToolTip",
                 "The suggested tile width when creating maps");

    ImGui::DragInt("Preferred tile height",
                   &mUiSettings.preferred_tile_size.y,
                   1.0f,
                   1,
                   10'000);
    lazy_tooltip("##PreferredTileHeightToolTip",
                 "The suggested tile height when creating maps");

    // TODO "RMB with stamp tool works as eraser"

    if (auto capacity = static_cast<int>(mUiSettings.command_capacity);
        ImGui::DragInt("Command capacity", &capacity, 1.0f, 10, 1'000)) {
      mUiSettings.command_capacity = static_cast<usize>(capacity);
    }

    lazy_tooltip("##CommandCapacityTooltip",
                 "The maximum amount of commands that will be stored on the undo stack");
  }
}

void SettingsDialog::update_appearance_tab()
{
  if (TabItem item {"Appearance"}; item.is_open()) {
    ImGui::Spacing();

    if (button("Restore Defaults")) {
      _reset_appearance_preferences(mUiSettings);
      _update_preview_settings(mUiSettings);
    }

    ImGui::Spacing();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Theme:");
    ImGui::SameLine();
    if (Combo combo {"##Theme", human_readable_name(mUiSettings.theme).data()};
        combo.is_open()) {
      const auto showThemes = [this](auto& themes) {
        for (const auto theme : themes) {
          if (ImGui::Selectable(human_readable_name(theme).data())) {
            mUiSettings.theme = theme;
            apply_theme(ImGui::GetStyle(), theme);
          }
        }
      };

      showThemes(light_themes);
      ImGui::Separator();
      showThemes(dark_themes);
    }

    if (auto arr = color_to_array(mUiSettings.viewport_background);
        ImGui::ColorEdit3("Viewport background color",
                          arr.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
      const auto color = cen::color::from_norm(arr.at(0), arr.at(1), arr.at(2));
      mUiSettings.viewport_background = color;
    }

    if (ImGui::Checkbox("Window border", &mUiSettings.window_border)) {
      ImGui::GetStyle().WindowBorderSize = mUiSettings.window_border ? 1.0f : 0.0f;
    }

    checkbox("Restore layout",
             &mUiSettings.restore_layout,
             "Restore the previous layout of widgets at startup");

    ImGui::Spacing();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    checkbox("Use default font",
             &mUiSettings.use_default_font,
             "Use the built-in bitmap font (only supports one size)");

    {
      Disable disableIf {mUiSettings.use_default_font};

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted("Font size:");

      ImGui::SameLine();
      if (ImGui::DragInt("##FontSize",
                         &mUiSettings.font_size,
                         1.0f,
                         min_font_size,
                         max_font_size)) {
        // TODO fix issue when set to non power of two, and then increased/decrease with
        // shortcuts (which causes crash due to assertions)
        mUiSettings.font_size = mUiSettings.font_size - mUiSettings.font_size % 2;
      }
    }
  }
}

void SettingsDialog::update_export_tab()
{
  if (TabItem item {"Export"}; item.is_open()) {
    ImGui::Spacing();

    if (button("Restore Defaults")) {
      _reset_export_preferences(mUiSettings);
      _update_preview_settings(mUiSettings);
    }

    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Preferred Format:");
    ImGui::SameLine();
    if (Combo format("##PreferredFormat", mUiSettings.preferred_format.c_str());
        format.is_open()) {
      if (ImGui::MenuItem("YAML")) {
        mUiSettings.preferred_format = "YAML";
      }

      if (ImGui::MenuItem("JSON")) {
        mUiSettings.preferred_format = "JSON";
      }

      if (ImGui::MenuItem("XML (TMX)")) {
        mUiSettings.preferred_format = "XML";
      }
    }

    lazy_tooltip(
        "##PreferredFormatTooltip",
        "The save file format used if no file extension is specified when saving maps");

    checkbox("Embed tilesets",
             &mUiSettings.embed_tilesets,
             "Embed tileset data in map files");

    checkbox("Indent output",
             &mUiSettings.indent_output,
             "Controls whether or not save files are indented");

    checkbox("Fold tile data",
             &mUiSettings.fold_tile_data,
             "Make tile layer data easier for humans to edit, at the expense of space");
  }
}

}  // namespace tactile::ui
