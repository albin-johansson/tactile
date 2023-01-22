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

#include "common/numeric.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/command_events.hpp"
#include "model/event/view_events.hpp"
#include "model/settings.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/fonts.hpp"
#include "ui/menu/menu.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/colors.hpp"
#include "ui/style/themes.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

struct SettingsDialogState final {
  Settings old_settings;
  Settings ui_settings;
  bool open_dialog {};
};

inline SettingsDialogState gDialogState;

void ui_flag_checkbox(const SettingsFlagBits flag,
                      const char* label,
                      const char* tooltip = nullptr)
{
  bool value = gDialogState.ui_settings.test_flag(flag);
  ui_checkbox(label, &value, tooltip);
  gDialogState.ui_settings.set_flag(flag, value);
}

void ui_lang_combo()
{
  if (const Combo combo {"##Lang",
                         get_language_name(gDialogState.ui_settings.get_language())};
      combo.is_open()) {
    if (ImGui::MenuItem(get_language_name(Lang::EN))) {
      gDialogState.ui_settings.set_language(Lang::EN);
    }

    if (ImGui::MenuItem(get_language_name(Lang::EN_GB))) {
      gDialogState.ui_settings.set_language(Lang::EN_GB);
    }

    if (ImGui::MenuItem(get_language_name(Lang::SV))) {
      gDialogState.ui_settings.set_language(Lang::SV);
    }
  }
}

void ui_theme_combo()
{
  auto show_themes = [](auto& themes) {
    for (const auto theme: themes) {
      if (Selectable::Property(human_readable_name(theme).data())) {
        gDialogState.ui_settings.set_theme(theme);
        apply_theme(ImGui::GetStyle(),
                    theme,
                    gDialogState.ui_settings.get_theme_saturation());
      }
    }
  };

  const auto current_theme = human_readable_name(gDialogState.ui_settings.get_theme());
  if (const Combo combo {"##Theme", current_theme.data()}; combo.is_open()) {
    show_themes(kLightThemes);
    ImGui::Separator();
    show_themes(kDarkThemes);
  }
}

void ui_map_format_combo()
{
  if (const Combo format("##PreferredFormat",
                         gDialogState.ui_settings.get_preferred_format().c_str());
      format.is_open()) {
    if (ImGui::MenuItem("YAML")) {
      gDialogState.ui_settings.set_preferred_format("YAML");
    }

    if (ImGui::MenuItem("JSON")) {
      gDialogState.ui_settings.set_preferred_format("JSON");
    }

    if (ImGui::MenuItem("XML (TMX)")) {
      gDialogState.ui_settings.set_preferred_format("XML");
    }
  }
}

void update_preview_settings(const Settings& settings)
{
  apply_theme(ImGui::GetStyle(), settings.get_theme(), settings.get_theme_saturation());
  ImGui::GetStyle().WindowBorderSize =
      settings.test_flag(SETTINGS_WINDOW_BORDER_BIT) ? 1.0f : 0.0f;
}

void apply_current_settings(entt::dispatcher& dispatcher)
{
  get_settings().copy_values_from(gDialogState.ui_settings);

  if (gDialogState.ui_settings.get_language() !=
      gDialogState.old_settings.get_language()) {
    reset_layout();
    menu_translate(get_current_language());
  }

  if (gDialogState.ui_settings.get_command_capacity() !=
      gDialogState.old_settings.get_command_capacity()) {
    dispatcher.enqueue<SetCommandCapacityEvent>(
        gDialogState.ui_settings.get_command_capacity());
  }

  if (gDialogState.ui_settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT) !=
          gDialogState.old_settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT) ||
      gDialogState.ui_settings.get_font_size() !=
          gDialogState.old_settings.get_font_size()) {
    dispatcher.enqueue<ReloadFontsEvent>();
  }

  gDialogState.old_settings.copy_values_from(gDialogState.ui_settings);
}

void update_behavior_tab(const Strings& lang)
{
  if (const TabItem tab {lang.setting.behavior_tab.c_str()}; tab.is_open()) {
    ImGui::Spacing();

    if (ui_button(lang.setting.restore_defaults.c_str())) {
      gDialogState.ui_settings.reset_behavior_values();
      update_preview_settings(gDialogState.ui_settings);
    }

    ImGui::Spacing();

    ui_flag_checkbox(SETTINGS_RESTORE_LAST_SESSION_BIT,
                     lang.setting.restore_last_session.c_str());

    auto preferred_tile_size = gDialogState.ui_settings.get_preferred_tile_size();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_tile_width.c_str());
    ImGui::SameLine();
    ImGui::DragInt("##PreferredTileWidth", &preferred_tile_size.x, 1.0f, 1, 10'000);
    ui_lazy_tooltip("##PreferredTileWidthToolTip", lang.tooltip.pref_tile_width.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_tile_height.c_str());
    ImGui::SameLine();
    ImGui::DragInt("##PreferredTileHeight", &preferred_tile_size.y, 1.0f, 1, 10'000);
    ui_lazy_tooltip("##PreferredTileHeightToolTip",
                    lang.tooltip.pref_tile_height.c_str());

    gDialogState.ui_settings.set_preferred_tile_size(preferred_tile_size);

    // TODO "RMB with stamp tool works as eraser"

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.command_capacity.c_str());
    ImGui::SameLine();

    uint64 command_capacity = gDialogState.ui_settings.get_command_capacity();
    const uint64 min_cmd_capacity = 10;
    const uint64 max_cmd_capacity = 1'000;
    ImGui::DragScalar("##CommandCapacity",
                      ImGuiDataType_U64,
                      &command_capacity,
                      1.0f,
                      &min_cmd_capacity,
                      &max_cmd_capacity);
    ui_lazy_tooltip("##CommandCapacityTooltip", lang.tooltip.command_capacity.c_str());

    gDialogState.ui_settings.set_command_capacity(command_capacity);
  }
}

void update_appearance_tab(const Strings& lang)
{
  if (const TabItem tab {lang.setting.appearance_tab.c_str()}; tab.is_open()) {
    ImGui::Spacing();

    if (ui_button(lang.setting.restore_defaults.c_str())) {
      gDialogState.ui_settings.reset_appearance_values();
      update_preview_settings(gDialogState.ui_settings);
    }

    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.language.c_str());
    ImGui::SameLine();
    right_align_next_item();
    ui_lang_combo();

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.theme.c_str());
    ImGui::SameLine();
    right_align_next_item();
    ui_theme_combo();

    {
      const Disable no_saturation_for_default_themes {
          gDialogState.ui_settings.get_theme() == EditorTheme::DearDark ||
          gDialogState.ui_settings.get_theme() == EditorTheme::DearLight};

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(lang.setting.saturation.c_str());
      ImGui::SameLine();

      int32 saturation = gDialogState.ui_settings.get_theme_saturation();
      const int32 min_saturation = 0;
      const int32 max_saturation = 100;
      if (ImGui::SliderScalar("##Saturation",
                              ImGuiDataType_S32,
                              &saturation,
                              &min_saturation,
                              &max_saturation)) {
        gDialogState.ui_settings.set_theme_saturation(saturation);
        update_preview_settings(gDialogState.ui_settings);
      }
    }

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    if (auto rgba = gDialogState.ui_settings.get_viewport_bg_color().as_float_array();
        ImGui::ColorEdit3(lang.setting.viewport_bg_color.c_str(),
                          rgba.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
      const auto color = Color::from_norm(rgba.at(0), rgba.at(1), rgba.at(2));
      gDialogState.ui_settings.set_viewport_bg_color(color);
    }

    if (auto rgba = gDialogState.ui_settings.get_grid_color().as_float_array();
        ImGui::ColorEdit4(lang.setting.grid_color.c_str(),
                          rgba.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar |
                              ImGuiColorEditFlags_AlphaPreviewHalf)) {
      const auto color = Color::from_norm(rgba.at(0), rgba.at(1), rgba.at(2), rgba.at(3));
      gDialogState.ui_settings.set_grid_color(color);
    }

    bool use_window_border =
        gDialogState.ui_settings.test_flag(SETTINGS_WINDOW_BORDER_BIT);
    if (ImGui::Checkbox(lang.setting.window_border.c_str(), &use_window_border)) {
      gDialogState.ui_settings.set_flag(SETTINGS_WINDOW_BORDER_BIT, use_window_border);
      ImGui::GetStyle().WindowBorderSize = use_window_border ? 1.0f : 0.0f;
    }

    ui_flag_checkbox(SETTINGS_RESTORE_LAYOUT_BIT,
                     lang.setting.restore_layout.c_str(),
                     lang.tooltip.restore_layout.c_str());

    ImGui::Spacing();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    ui_flag_checkbox(SETTINGS_USE_DEFAULT_FONT_BIT,
                     lang.setting.use_default_font.c_str(),
                     lang.tooltip.use_default_font.c_str());

    {
      const Disable when_using_default_font {
          gDialogState.ui_settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT)};

      int32 font_size = gDialogState.ui_settings.get_font_size();

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
        gDialogState.ui_settings.set_font_size(font_size - font_size % 2);
      }
    }
  }
}

void update_export_tab(const Strings& lang)
{
  if (const TabItem tab {lang.setting.export_tab.c_str()}; tab.is_open()) {
    ImGui::Spacing();

    if (ui_button(lang.setting.restore_defaults.c_str())) {
      gDialogState.ui_settings.reset_export_values();
      update_preview_settings(gDialogState.ui_settings);
    }

    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.setting.pref_format.c_str());
    ImGui::SameLine();
    right_align_next_item();
    ui_map_format_combo();
    ui_lazy_tooltip("##PreferredFormatTooltip", lang.tooltip.pref_format.c_str());

    ui_flag_checkbox(SETTINGS_EMBED_TILESETS_BIT,
                     lang.setting.embed_tilesets.c_str(),
                     lang.tooltip.embed_tilesets.c_str());

    ui_flag_checkbox(SETTINGS_INDENT_OUTPUT_BIT,
                     lang.setting.indent_output.c_str(),
                     lang.tooltip.indent_output.c_str());

    ui_flag_checkbox(SETTINGS_FOLD_TILE_DATA_BIT,
                     lang.setting.fold_tile_data.c_str(),
                     lang.tooltip.fold_tile_data.c_str());
  }
}

}  // namespace

void open_settings_dialog()
{
  gDialogState.old_settings.copy_values_from(get_settings());
  gDialogState.ui_settings.copy_values_from(gDialogState.old_settings);

  gDialogState.open_dialog = true;
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

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
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
    update_preview_settings(get_settings());
  }
  else if (action == DialogAction::Cancel) {
    // Reset any changes we made for preview purposes
    update_preview_settings(get_settings());
  }
}

}  // namespace tactile::ui
