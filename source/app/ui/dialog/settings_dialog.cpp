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

#include "core/vocabulary.hpp"
#include "editor/menu/menu.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/command_events.hpp"
#include "model/event/view_events.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/fonts.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/colors.hpp"
#include "ui/style/themes.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

inline io::PreferenceState dialog_old_settings;
inline io::PreferenceState dialog_ui_settings;
inline constinit bool open_dialog = false;

void ui_lang_combo()
{
  if (const Combo combo {"##Lang", get_language_name(dialog_ui_settings.language)};
      combo.is_open()) {
    if (ImGui::MenuItem(get_language_name(Lang::EN))) {
      dialog_ui_settings.language = Lang::EN;
    }

    if (ImGui::MenuItem(get_language_name(Lang::EN_GB))) {
      dialog_ui_settings.language = Lang::EN_GB;
    }

    if (ImGui::MenuItem(get_language_name(Lang::SV))) {
      dialog_ui_settings.language = Lang::SV;
    }
  }
}

void ui_theme_combo()
{
  auto show_themes = [](auto& themes) {
    for (const auto theme: themes) {
      if (Selectable::Property(human_readable_name(theme).data())) {
        dialog_ui_settings.theme = theme;
        apply_theme(ImGui::GetStyle(), theme);
      }
    }
  };

  const auto current_theme = human_readable_name(dialog_ui_settings.theme);
  if (const Combo combo {"##Theme", current_theme.data()}; combo.is_open()) {
    show_themes(light_themes);
    ImGui::Separator();
    show_themes(dark_themes);
  }
}

void ui_map_format_combo()
{
  if (const Combo format("##PreferredFormat",
                         dialog_ui_settings.preferred_format.c_str());
      format.is_open()) {
    if (ImGui::MenuItem("YAML")) {
      dialog_ui_settings.preferred_format = "YAML";
    }

    if (ImGui::MenuItem("JSON")) {
      dialog_ui_settings.preferred_format = "JSON";
    }

    if (ImGui::MenuItem("XML (TMX)")) {
      dialog_ui_settings.preferred_format = "XML";
    }
  }
}

void reset_appearance_preferences(io::PreferenceState& prefs)
{
  prefs.language = io::def_language;
  prefs.theme = io::def_theme;
  prefs.viewport_background = io::def_viewport_bg;
  prefs.grid_color = io::def_grid_color;

  prefs.window_border = io::def_window_border;
  prefs.show_grid = io::def_show_grid;

  prefs.restore_layout = io::def_restore_layout;
  prefs.use_default_font = io::def_use_default_font;
  prefs.font_size = ui::def_font_size;
}

void reset_behavior_preferences(io::PreferenceState& prefs)
{
  prefs.command_capacity = io::def_command_capacity;
  prefs.preferred_tile_size = io::def_preferred_tile_size;
  prefs.restore_last_session = io::def_restore_last_session;
}

void reset_export_preferences(io::PreferenceState& prefs)
{
  prefs.preferred_format = io::def_preferred_format;
  prefs.embed_tilesets = io::def_embed_tilesets;
  prefs.indent_output = io::def_indent_output;
  prefs.fold_tile_data = io::def_fold_tile_data;
}

void update_preview_settings(const io::PreferenceState& prefs)
{
  apply_theme(ImGui::GetStyle(), prefs.theme);
  ImGui::GetStyle().WindowBorderSize = prefs.window_border ? 1.0f : 0.0f;
}

void apply_current_settings(entt::dispatcher& dispatcher)
{
  io::set_preferences(dialog_ui_settings);

  if (dialog_ui_settings.language != dialog_old_settings.language) {
    reset_layout();
    menu_translate(get_current_language());
  }

  if (dialog_ui_settings.command_capacity != dialog_old_settings.command_capacity) {
    dispatcher.enqueue<SetCommandCapacityEvent>(dialog_ui_settings.command_capacity);
  }

  if (dialog_ui_settings.use_default_font != dialog_old_settings.use_default_font ||
      dialog_ui_settings.font_size != dialog_old_settings.font_size) {
    dispatcher.enqueue<ReloadFontsEvent>();
  }

  dialog_old_settings = dialog_ui_settings;
}

void update_behavior_tab(const Strings& lang)
{
  if (const TabItem tab {lang.setting.behavior_tab.c_str()}; tab.is_open()) {
    ImGui::Spacing();

    if (ui_button(lang.setting.restore_defaults.c_str())) {
      reset_behavior_preferences(dialog_ui_settings);
      update_preview_settings(dialog_ui_settings);
    }

    ImGui::Spacing();

    ui_checkbox(lang.setting.restore_last_session.c_str(),
                &dialog_ui_settings.restore_last_session);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_tile_width.c_str());
    ImGui::SameLine();
    ImGui::DragInt("##PreferredTileWidth",
                   &dialog_ui_settings.preferred_tile_size.x,
                   1.0f,
                   1,
                   10'000);
    ui_lazy_tooltip("##PreferredTileWidthToolTip", lang.tooltip.pref_tile_width.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_tile_height.c_str());
    ImGui::SameLine();
    ImGui::DragInt("##PreferredTileHeight",
                   &dialog_ui_settings.preferred_tile_size.y,
                   1.0f,
                   1,
                   10'000);
    ui_lazy_tooltip("##PreferredTileHeightToolTip",
                    lang.tooltip.pref_tile_height.c_str());

    // TODO "RMB with stamp tool works as eraser"

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.command_capacity.c_str());
    ImGui::SameLine();
    if (auto capacity = static_cast<int>(dialog_ui_settings.command_capacity);
        ImGui::DragInt("##CommandCapacity", &capacity, 1.0f, 10, 1'000)) {
      dialog_ui_settings.command_capacity = static_cast<usize>(capacity);
    }

    ui_lazy_tooltip("##CommandCapacityTooltip", lang.tooltip.command_capacity.c_str());
  }
}

void update_appearance_tab(const Strings& lang)
{
  if (const TabItem tab {lang.setting.appearance_tab.c_str()}; tab.is_open()) {
    ImGui::Spacing();

    if (ui_button(lang.setting.restore_defaults.c_str())) {
      reset_appearance_preferences(dialog_ui_settings);
      update_preview_settings(dialog_ui_settings);
    }

    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.language.c_str());
    ImGui::SameLine();
    right_align_next_item();
    ui_lang_combo();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.theme.c_str());
    ImGui::SameLine();
    right_align_next_item();
    ui_theme_combo();

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    if (auto rgba = dialog_ui_settings.viewport_background.as_float_array();
        ImGui::ColorEdit3(lang.setting.viewport_bg_color.c_str(),
                          rgba.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
      const auto color = Color::from_norm(rgba.at(0), rgba.at(1), rgba.at(2));
      dialog_ui_settings.viewport_background = color;
    }

    if (auto rgba = dialog_ui_settings.grid_color.as_float_array();
        ImGui::ColorEdit4(lang.setting.grid_color.c_str(),
                          rgba.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar |
                              ImGuiColorEditFlags_AlphaPreviewHalf)) {
      const auto color = Color::from_norm(rgba.at(0), rgba.at(1), rgba.at(2), rgba.at(3));
      dialog_ui_settings.grid_color = color;
    }

    if (ImGui::Checkbox(lang.setting.window_border.c_str(),
                        &dialog_ui_settings.window_border)) {
      ImGui::GetStyle().WindowBorderSize = dialog_ui_settings.window_border ? 1.0f : 0.0f;
    }

    ui_checkbox(lang.setting.restore_layout.c_str(),
                &dialog_ui_settings.restore_layout,
                lang.tooltip.restore_layout.c_str());

    ImGui::Spacing();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    ui_checkbox(lang.setting.use_default_font.c_str(),
                &dialog_ui_settings.use_default_font,
                lang.tooltip.use_default_font.c_str());

    {
      const Disable when_using_default_font {dialog_ui_settings.use_default_font};

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(lang.setting.font_size.c_str());

      ImGui::SameLine();
      if (ImGui::DragInt("##FontSize",
                         &dialog_ui_settings.font_size,
                         1.0f,
                         min_font_size,
                         max_font_size)) {
        // TODO fix issue when set to non power of two, and then increased/decrease with
        // shortcuts (which causes crash due to assertions)
        dialog_ui_settings.font_size =
            dialog_ui_settings.font_size - dialog_ui_settings.font_size % 2;
      }
    }
  }
}

void update_export_tab(const Strings& lang)
{
  if (const TabItem tab {lang.setting.export_tab.c_str()}; tab.is_open()) {
    ImGui::Spacing();

    if (ui_button(lang.setting.restore_defaults.c_str())) {
      reset_export_preferences(dialog_ui_settings);
      update_preview_settings(dialog_ui_settings);
    }

    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_format.c_str());
    ImGui::SameLine();
    right_align_next_item();
    ui_map_format_combo();
    ui_lazy_tooltip("##PreferredFormatTooltip", lang.tooltip.pref_format.c_str());

    ui_checkbox(lang.setting.embed_tilesets.c_str(),
                &dialog_ui_settings.embed_tilesets,
                lang.tooltip.embed_tilesets.c_str());

    ui_checkbox(lang.setting.indent_output.c_str(),
                &dialog_ui_settings.indent_output,
                lang.tooltip.indent_output.c_str());

    ui_checkbox(lang.setting.fold_tile_data.c_str(),
                &dialog_ui_settings.fold_tile_data,
                lang.tooltip.fold_tile_data.c_str());
  }
}

}  // namespace

void open_settings_dialog()
{
  dialog_old_settings = io::get_preferences();
  dialog_ui_settings = dialog_old_settings;

  open_dialog = true;
}

void update_settings_dialog(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.settings_dialog.c_str(),
      .close_label = lang.misc.cancel.c_str(),
      .accept_label = lang.misc.ok.c_str(),
      .apply_label = lang.misc.apply.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    open_dialog = false;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    if (const TabBar bar {"##SettingsTabs"}; bar.is_open()) {
      update_behavior_tab(lang);
      update_appearance_tab(lang);
      update_export_tab(lang);
    }
  }

  if (action == DialogAction::Apply || action == DialogAction::Accept) {
    apply_current_settings(dispatcher);
    update_preview_settings(io::get_preferences());
  }
  else if (action == DialogAction::Cancel) {
    // Reset any changes we made for preview purposes
    update_preview_settings(io::get_preferences());
  }
}

}  // namespace tactile::ui
