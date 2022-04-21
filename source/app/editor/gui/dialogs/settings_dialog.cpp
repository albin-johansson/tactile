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

#include "cfg/fonts.hpp"
#include "core/utils/colors.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/checkbox.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/gui/themes.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {
namespace {

void _update_preview_settings(const PreferenceState& prefs)
{
  apply_theme(ImGui::GetStyle(), prefs.get_theme());
  ImGui::GetStyle().WindowBorderSize = prefs.has_window_border() ? 1.0f : 0.0f;
}

}  // namespace

SettingsDialog::SettingsDialog() : ADialog{"Settings"}
{
  use_apply_button();
}

void SettingsDialog::show()
{
  mSnapshot = get_preferences();
  mGuiSettings = mSnapshot;
  make_visible();
}

void SettingsDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  if (scoped::TabBar bar{"##SettingsTabBar"}; bar.is_open()) {
    update_behavior_tab();
    update_appearance_tab();
    update_export_tab();
  }
}

void SettingsDialog::on_cancel()
{
  /* Reset any changes we made for preview purposes */
  _update_preview_settings(get_preferences());
}

void SettingsDialog::on_accept(entt::dispatcher& dispatcher)
{
  apply_settings(dispatcher);
  _update_preview_settings(get_preferences());
}

void SettingsDialog::on_apply(entt::dispatcher& dispatcher)
{
  apply_settings(dispatcher);
  _update_preview_settings(get_preferences());
}

void SettingsDialog::apply_settings(entt::dispatcher& dispatcher)
{
  set_preferences(mGuiSettings);

  if (mGuiSettings.command_capacity() != mSnapshot.command_capacity()) {
    dispatcher.enqueue<SetCommandCapacityEvent>(mGuiSettings.command_capacity());
  }

  if (mGuiSettings.use_default_font() != mSnapshot.use_default_font() ||
      mGuiSettings.font_size() != mSnapshot.font_size()) {
    dispatcher.enqueue<ReloadFontsEvent>();
  }

  mSnapshot = mGuiSettings;
}

void SettingsDialog::update_behavior_tab()
{
  if (scoped::TabItem item{"Behavior"}; item.is_open()) {
    ImGui::Spacing();
    if (button("Restore Defaults")) {
      mGuiSettings.reset_behavior_preferences();
      _update_preview_settings(mGuiSettings);
    }
    ImGui::Spacing();

    if (bool restore = mGuiSettings.will_restore_last_session();
        checkbox("Restore last session on startup", &restore)) {
      mGuiSettings.set_will_restore_last_session(restore);
    }

    if (auto width = mGuiSettings.preferred_tile_width();
        ImGui::DragInt("Preferred tile width", &width, 1.0f, 1, 10'000)) {
      mGuiSettings.set_preferred_tile_width(width);
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile width when creating maps");
    }

    if (auto height = mGuiSettings.preferred_tile_height();
        ImGui::DragInt("Preferred tile height", &height, 1.0f, 1, 10'000)) {
      mGuiSettings.set_preferred_tile_height(height);
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile height when creating maps");
    }

    // TODO "RMB with stamp tool works as eraser"

    if (auto capacity = static_cast<int>(mGuiSettings.command_capacity());
        ImGui::DragInt("Command capacity", &capacity, 1.0f, 10, 1'000)) {
      mGuiSettings.set_command_capacity(static_cast<usize>(capacity));
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The maximum amount of commands that will be stored on the undo stack");
    }
  }
}

void SettingsDialog::update_appearance_tab()
{
  if (scoped::TabItem item{"Appearance"}; item.is_open()) {
    ImGui::Spacing();

    if (button("Restore Defaults")) {
      mGuiSettings.reset_appearance_preferences();
      _update_preview_settings(mGuiSettings);
    }

    ImGui::Spacing();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    if (scoped::Combo combo{"Theme",
                            human_readable_name(mGuiSettings.get_theme()).data()};
        combo.is_open()) {
      for (const auto theme : themes) {
        if (ImGui::Selectable(human_readable_name(theme).data())) {
          mGuiSettings.set_theme(theme);
          apply_theme(ImGui::GetStyle(), theme);
        }
      }
    }

    if (auto arr = color_to_array(mGuiSettings.viewport_bg());
        ImGui::ColorEdit3("Viewport background color",
                          arr.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
      mGuiSettings.set_viewport_bg(
          cen::color::from_norm(arr.at(0), arr.at(1), arr.at(2)));
    }

    if (bool enabled = mGuiSettings.has_window_border();
        ImGui::Checkbox("Window border", &enabled)) {
      mGuiSettings.set_window_border(enabled);
      ImGui::GetStyle().WindowBorderSize = enabled ? 1.0f : 0.0f;
    }

    if (bool restore = mGuiSettings.will_restore_layout();
        checkbox("Restore layout",
                 &restore,
                 "Restore the previous layout of widgets at startup")) {
      mGuiSettings.set_will_restore_layout(restore);
    }

    ImGui::Spacing();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    if (bool use = mGuiSettings.use_default_font();
        checkbox("Use default font",
                 &use,
                 "Use the built-in bitmap font (only supports one size)")) {
      mGuiSettings.set_use_default_font(use);
    }

    {
      scoped::Disable disableIf{mGuiSettings.use_default_font()};

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted("Font size:");

      ImGui::SameLine();
      if (int size = mGuiSettings.font_size();  //
          ImGui::DragInt("##FontSize",
                         &size,
                         1.0f,
                         get_min_font_size(),
                         get_max_font_size())) {
        // TODO fix issue when set to non power of two, and then increased/decrease with
        // shortcuts (which causes crash due to assertions)
        mGuiSettings.set_font_size(size - size % 2);
      }
    }
  }
}

void SettingsDialog::update_export_tab()
{
  if (scoped::TabItem item{"Export"}; item.is_open()) {
    ImGui::Spacing();

    if (button("Restore Defaults")) {
      mGuiSettings.reset_export_preferences();
      _update_preview_settings(mGuiSettings);
    }

    ImGui::Spacing();

    if (scoped::Combo format("Preferred Format", mGuiSettings.preferred_format().c_str());
        format.is_open()) {
      if (ImGui::MenuItem("YAML")) {
        mGuiSettings.set_preferred_format("YAML");
      }

      if (ImGui::MenuItem("JSON")) {
        mGuiSettings.set_preferred_format("JSON");
      }

      if (ImGui::MenuItem("XML (TMX)")) {
        mGuiSettings.set_preferred_format("XML");
      }
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The save file format used if no file extension is specified when saving maps");
    }

    if (bool embed = mGuiSettings.embed_tilesets();
        checkbox("Embed tilesets", &embed, "Embed tileset data in map files")) {
      mGuiSettings.set_embed_tilesets(embed);
    }

    if (bool indent = mGuiSettings.indent_output();
        checkbox("Indent output",
                 &indent,
                 "Controls whether or not save files are indented")) {
      mGuiSettings.set_indent_output(indent);
    }

    if (bool fold = mGuiSettings.fold_tile_data();  //
        checkbox(
            "Fold tile data",
            &fold,
            "Make tile layer data easier for humans to edit, at the expense of space")) {
      mGuiSettings.set_fold_tile_data(fold);
    }
  }
}

}  // namespace tactile
