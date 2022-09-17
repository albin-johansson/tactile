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

#include "core/event/command_events.hpp"
#include "core/event/misc_events.hpp"
#include "editor/ui/alignment.hpp"
#include "editor/ui/common/buttons.hpp"
#include "editor/ui/common/checkboxes.hpp"
#include "editor/ui/common/colors.hpp"
#include "editor/ui/common/tooltips.hpp"
#include "editor/ui/dock_space.hpp"
#include "editor/ui/fonts.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/themes.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

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

SettingsDialog::SettingsDialog()
    : Dialog {get_current_language().window.settings_dialog}
{
  use_apply_button();
}

void SettingsDialog::show()
{
  mSnapshot = io::get_preferences();
  mUiSettings = mSnapshot;

  set_title(get_current_language().window.settings_dialog);
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

  if (mUiSettings.language != mSnapshot.language) {
    reset_layout();
  }

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
  const auto& lang = get_current_language();

  if (TabItem item {lang.setting.behavior_tab.c_str()}; item.is_open()) {
    ImGui::Spacing();

    if (button(lang.setting.restore_defaults.c_str())) {
      _reset_behavior_preferences(mUiSettings);
      _update_preview_settings(mUiSettings);
    }

    ImGui::Spacing();

    checkbox(lang.setting.restore_last_session.c_str(),
             &mUiSettings.restore_last_session);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_tile_width.c_str());
    ImGui::SameLine();
    ImGui::DragInt("##PreferredTileWidth",
                   &mUiSettings.preferred_tile_size.x,
                   1.0f,
                   1,
                   10'000);
    lazy_tooltip("##PreferredTileWidthToolTip", lang.tooltip.pref_tile_width.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_tile_height.c_str());
    ImGui::SameLine();
    ImGui::DragInt("##PreferredTileHeight",
                   &mUiSettings.preferred_tile_size.y,
                   1.0f,
                   1,
                   10'000);
    lazy_tooltip("##PreferredTileHeightToolTip", lang.tooltip.pref_tile_height.c_str());

    // TODO "RMB with stamp tool works as eraser"

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.command_capacity.c_str());
    ImGui::SameLine();
    if (auto capacity = static_cast<int>(mUiSettings.command_capacity);
        ImGui::DragInt("##CommandCapacity", &capacity, 1.0f, 10, 1'000)) {
      mUiSettings.command_capacity = static_cast<usize>(capacity);
    }

    lazy_tooltip("##CommandCapacityTooltip", lang.tooltip.command_capacity.c_str());
  }
}

void SettingsDialog::update_appearance_tab()
{
  const auto& lang = get_current_language();

  if (TabItem item {lang.setting.appearance_tab.c_str()}; item.is_open()) {
    ImGui::Spacing();

    if (button(lang.setting.restore_defaults.c_str())) {
      _reset_appearance_preferences(mUiSettings);
      _update_preview_settings(mUiSettings);
    }

    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.language.c_str());
    ImGui::SameLine();
    right_align_next_item();
    if (Combo combo {"##Lang", get_language_name(mUiSettings.language)};
        combo.is_open()) {
      if (ImGui::MenuItem(get_language_name(Lang::EN))) {
        mUiSettings.language = Lang::EN;
      }

      if (ImGui::MenuItem(get_language_name(Lang::EN_GB))) {
        mUiSettings.language = Lang::EN_GB;
      }

      if (ImGui::MenuItem(get_language_name(Lang::SV))) {
        mUiSettings.language = Lang::SV;
      }
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.theme.c_str());
    ImGui::SameLine();
    right_align_next_item();
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

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    if (auto arr = color_to_array(mUiSettings.viewport_background);
        ImGui::ColorEdit3(lang.setting.viewport_bg_color.c_str(),
                          arr.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
      const auto color = cen::color::from_norm(arr.at(0), arr.at(1), arr.at(2));
      mUiSettings.viewport_background = color;
    }

    if (ImGui::Checkbox(lang.setting.window_border.c_str(), &mUiSettings.window_border)) {
      ImGui::GetStyle().WindowBorderSize = mUiSettings.window_border ? 1.0f : 0.0f;
    }

    checkbox(lang.setting.restore_layout.c_str(),
             &mUiSettings.restore_layout,
             lang.tooltip.restore_layout.c_str());

    ImGui::Spacing();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    checkbox(lang.setting.use_default_font.c_str(),
             &mUiSettings.use_default_font,
             lang.tooltip.use_default_font.c_str());

    {
      Disable disableIf {mUiSettings.use_default_font};

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(lang.setting.font_size.c_str());

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
  const auto& lang = get_current_language();

  if (TabItem item {lang.setting.export_tab.c_str()}; item.is_open()) {
    ImGui::Spacing();

    if (button(lang.setting.restore_defaults.c_str())) {
      _reset_export_preferences(mUiSettings);
      _update_preview_settings(mUiSettings);
    }

    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_format.c_str());
    ImGui::SameLine();
    right_align_next_item();
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

    lazy_tooltip("##PreferredFormatTooltip", lang.tooltip.pref_format.c_str());

    checkbox(lang.setting.embed_tilesets.c_str(),
             &mUiSettings.embed_tilesets,
             lang.tooltip.embed_tilesets.c_str());

    checkbox(lang.setting.indent_output.c_str(),
             &mUiSettings.indent_output,
             lang.tooltip.indent_output.c_str());

    checkbox(lang.setting.fold_tile_data.c_str(),
             &mUiSettings.fold_tile_data,
             lang.tooltip.fold_tile_data.c_str());
  }
}

}  // namespace tactile::ui
