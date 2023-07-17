/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <imgui.h>
#include <imgui_internal.h>

#include "common/primitives.hpp"
#include "io/save_formats.hpp"
#include "model/events/command_events.hpp"
#include "model/events/setting_events.hpp"
#include "model/events/view_events.hpp"
#include "model/i18n/language_system.hpp"
#include "ui/constants.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/colors.hpp"
#include "ui/style/themes.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

void _push_flag_checkbox(SettingsDialogState& state,
                         const SettingsFlagBits flag,
                         const char* label,
                         const char* tooltip = nullptr)
{
  bool value = state.ui_settings.test_flag(flag);
  push_checkbox(label, &value, tooltip);
  state.ui_settings.set_flag(flag, value);
}

void _push_lang_combo(SettingsDialogState& state)
{
  if (const Combo combo {"##Lang",
                         sys::get_language_name(state.ui_settings.get_language())};
      combo.is_open()) {
    if (ImGui::MenuItem(sys::get_language_name(Lang::EN))) {
      state.ui_settings.set_language(Lang::EN);
    }

    if (ImGui::MenuItem(sys::get_language_name(Lang::EN_GB))) {
      state.ui_settings.set_language(Lang::EN_GB);
    }

    if (ImGui::MenuItem(sys::get_language_name(Lang::SV))) {
      state.ui_settings.set_language(Lang::SV);
    }
  }
}

void _push_theme_combo(SettingsDialogState& state)
{
  auto show_themes = [&](const auto& themes) {
    for (const auto theme: themes) {
      if (Selectable::property(human_readable_name(theme).data())) {
        state.ui_settings.set_theme(theme);
        apply_theme(ImGui::GetStyle(), theme, state.ui_settings.get_theme_saturation());
      }
    }
  };

  const auto current_theme = human_readable_name(state.ui_settings.get_theme());
  if (const Combo combo {"##Theme", current_theme.data()}; combo.is_open()) {
    show_themes(kLightThemes);
    ImGui::Separator();
    show_themes(kDarkThemes);
  }
}

void _push_map_format_combo(SettingsDialogState& state)
{
  const auto preferred_format = state.ui_settings.get_preferred_format();
  if (const Combo format("##PreferredFormat",
                         get_human_readable_name(preferred_format).data());
      format.is_open()) {
    if (ImGui::MenuItem(get_human_readable_name(SaveFormat::TactileYaml).data())) {
      state.ui_settings.set_preferred_format(SaveFormat::TactileYaml);
    }

    if (ImGui::MenuItem(get_human_readable_name(SaveFormat::TiledJson).data())) {
      state.ui_settings.set_preferred_format(SaveFormat::TiledJson);
    }

    if (ImGui::MenuItem(get_human_readable_name(SaveFormat::TiledXml).data())) {
      state.ui_settings.set_preferred_format(SaveFormat::TiledXml);
    }
  }
}

void _apply_settings_preview(const Settings& settings)
{
  apply_theme(ImGui::GetStyle(), settings.get_theme(), settings.get_theme_saturation());
  ImGui::GetStyle().WindowBorderSize =
      settings.test_flag(SETTINGS_WINDOW_BORDER_BIT) ? 1.0f : 0.0f;
}

void _apply_current_settings(SettingsDialogState& state, Dispatcher& dispatcher)
{
  dispatcher.enqueue<SetSettingsEvent>(state.ui_settings.copy());
  state.old_settings.copy_from(state.ui_settings);
}

void _push_behavior_tab(const Strings& lang, SettingsDialogState& state)
{
  if (const TabItem tab {lang.setting.behavior_tab.c_str()}; tab.is_open()) {
    ImGui::Spacing();

    if (push_button(lang.setting.restore_defaults.c_str())) {
      state.ui_settings.reset_behavior_values();
      _apply_settings_preview(state.ui_settings);
    }

    ImGui::Spacing();

    _push_flag_checkbox(state,
                        SETTINGS_RESTORE_LAST_SESSION_BIT,
                        lang.setting.restore_last_session.c_str());

    auto preferred_tile_size = state.ui_settings.get_preferred_tile_size();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_tile_width.c_str());
    ImGui::SameLine();
    ImGui::DragInt("##PreferredTileWidth", &preferred_tile_size.x, 1.0f, 1, 10'000);
    push_lazy_tooltip("##PreferredTileWidthToolTip",
                      lang.tooltip.pref_tile_width.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_tile_height.c_str());
    ImGui::SameLine();
    ImGui::DragInt("##PreferredTileHeight", &preferred_tile_size.y, 1.0f, 1, 10'000);
    push_lazy_tooltip("##PreferredTileHeightToolTip",
                      lang.tooltip.pref_tile_height.c_str());

    state.ui_settings.set_preferred_tile_size(preferred_tile_size);

    // TODO "RMB with stamp tool works as eraser"

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.command_capacity.c_str());
    ImGui::SameLine();

    uint64 command_capacity = state.ui_settings.get_command_capacity();
    const uint64 min_cmd_capacity = 10;
    const uint64 max_cmd_capacity = 1'000;
    ImGui::DragScalar("##CommandCapacity",
                      ImGuiDataType_U64,
                      &command_capacity,
                      1.0f,
                      &min_cmd_capacity,
                      &max_cmd_capacity);
    push_lazy_tooltip("##CommandCapacityTooltip", lang.tooltip.command_capacity.c_str());

    state.ui_settings.set_command_capacity(command_capacity);
  }
}

void _push_appearance_tab(const Strings& lang, SettingsDialogState& state)
{
  if (const TabItem tab {lang.setting.appearance_tab.c_str()}; tab.is_open()) {
    ImGui::Spacing();

    if (push_button(lang.setting.restore_defaults.c_str())) {
      state.ui_settings.reset_appearance_values();
      _apply_settings_preview(state.ui_settings);
    }

    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.language.c_str());
    ImGui::SameLine();
    right_align_next_item();
    _push_lang_combo(state);

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.theme.c_str());
    ImGui::SameLine();
    right_align_next_item();
    _push_theme_combo(state);

    {
      const Disable no_saturation_for_default_themes {
          state.ui_settings.get_theme() == Theme::DearDark ||
          state.ui_settings.get_theme() == Theme::DearLight};

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(lang.setting.saturation.c_str());
      ImGui::SameLine();

      int32 saturation = state.ui_settings.get_theme_saturation();
      const int32 min_saturation = 0;
      const int32 max_saturation = 100;
      if (ImGui::SliderScalar("##Saturation",
                              ImGuiDataType_S32,
                              &saturation,
                              &min_saturation,
                              &max_saturation)) {
        state.ui_settings.set_theme_saturation(saturation);
        _apply_settings_preview(state.ui_settings);
      }
    }

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    if (auto rgba = state.ui_settings.get_viewport_bg_color().as_float_array();
        ImGui::ColorEdit3(lang.setting.viewport_bg_color.c_str(),
                          rgba.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
      const auto color = Color::from_norm(rgba.at(0), rgba.at(1), rgba.at(2));
      state.ui_settings.set_viewport_bg_color(color);
    }

    if (auto rgba = state.ui_settings.get_grid_color().as_float_array();
        ImGui::ColorEdit4(lang.setting.grid_color.c_str(),
                          rgba.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar |
                              ImGuiColorEditFlags_AlphaPreviewHalf)) {
      const auto color = Color::from_norm(rgba.at(0), rgba.at(1), rgba.at(2), rgba.at(3));
      state.ui_settings.set_grid_color(color);
    }

    bool use_window_border = state.ui_settings.test_flag(SETTINGS_WINDOW_BORDER_BIT);
    if (ImGui::Checkbox(lang.setting.window_border.c_str(), &use_window_border)) {
      state.ui_settings.set_flag(SETTINGS_WINDOW_BORDER_BIT, use_window_border);
      ImGui::GetStyle().WindowBorderSize = use_window_border ? 1.0f : 0.0f;
    }

    _push_flag_checkbox(state,
                        SETTINGS_RESTORE_LAYOUT_BIT,
                        lang.setting.restore_layout.c_str(),
                        lang.tooltip.restore_layout.c_str());

    ImGui::Spacing();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    _push_flag_checkbox(state,
                        SETTINGS_USE_DEFAULT_FONT_BIT,
                        lang.setting.use_default_font.c_str(),
                        lang.tooltip.use_default_font.c_str());

    {
      const Disable when_using_default_font {
          state.ui_settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT)};

      int32 font_size = state.ui_settings.get_font_size();

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(lang.setting.font_size.c_str());
      ImGui::SameLine();
      if (ImGui::DragScalar("##FontSize",
                            ImGuiDataType_S32,
                            &font_size,
                            1.0f,
                            &kMinFontSize,
                            &kMaxFontSize)) {
        // TODO fix issue when set to non power of two, and then increased/decrease with
        // shortcuts (which causes crash due to assertions)
        state.ui_settings.set_font_size(font_size - font_size % 2);
      }
    }
  }
}

void _push_export_tab(const Strings& lang, SettingsDialogState& state)
{
  if (const TabItem tab {lang.setting.export_tab.c_str()}; tab.is_open()) {
    ImGui::Spacing();

    if (push_button(lang.setting.restore_defaults.c_str())) {
      state.ui_settings.reset_export_values();
      _apply_settings_preview(state.ui_settings);
    }

    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_format.c_str());
    ImGui::SameLine();
    right_align_next_item();
    _push_map_format_combo(state);
    push_lazy_tooltip("##PreferredFormatTooltip", lang.tooltip.pref_format.c_str());

    _push_flag_checkbox(state,
                        SETTINGS_EMBED_TILESETS_BIT,
                        lang.setting.embed_tilesets.c_str(),
                        lang.tooltip.embed_tilesets.c_str());

    _push_flag_checkbox(state,
                        SETTINGS_INDENT_OUTPUT_BIT,
                        lang.setting.indent_output.c_str(),
                        lang.tooltip.indent_output.c_str());

    _push_flag_checkbox(state,
                        SETTINGS_FOLD_TILE_DATA_BIT,
                        lang.setting.fold_tile_data.c_str(),
                        lang.tooltip.fold_tile_data.c_str());
  }
}

}  // namespace

void push_settings_dialog(const Registry& registry,
                          SettingsDialogState& state,
                          Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(registry);

  DialogOptions dialog_options {
      .title = strings.window.settings_dialog.c_str(),
      .close_label = strings.misc.cancel.c_str(),
      .accept_label = strings.misc.ok.c_str(),
      .apply_label = strings.misc.apply.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (state.should_open) {
    dialog_options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {dialog_options, &action}; dialog.was_opened()) {
    if (const TabBar bar {"##SettingsTabs"}; bar.is_open()) {
      _push_behavior_tab(strings, state);
      _push_appearance_tab(strings, state);
      _push_export_tab(strings, state);
    }
  }

  if (action == DialogAction::Apply || action == DialogAction::Accept) {
    _apply_settings_preview(state.ui_settings);
    _apply_current_settings(state, dispatcher);
  }
  else if (action == DialogAction::Cancel) {
    // Reset any changes we made for preview purposes
    _apply_settings_preview(state.old_settings);
  }
}

}  // namespace tactile::ui
