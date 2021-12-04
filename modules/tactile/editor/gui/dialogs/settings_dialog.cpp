#include "settings_dialog.hpp"

#include <imgui.h>
#include <magic_enum.hpp>  // enum_name

#include "core/utils/color_utils.hpp"
#include "editor/events/command_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/checkbox.hpp"
#include "editor/gui/common/combo.hpp"
#include "editor/gui/common/modal.hpp"
#include "editor/gui/themes.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constexpr auto gFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit bool gIsVisible = false;

inline Preferences gSnapshot;  // The original settings when the dialog was opened
inline Preferences gSettings;  // The value of the settings in the GUI

void UpdatePreviewSettings(const Preferences& prefs)
{
  ApplyTheme(ImGui::GetStyle(), prefs.theme);
  ImGui::GetStyle().WindowBorderSize =
      (prefs.flags & Preferences::window_border) ? 1.0f : 0.0f;
}

void ShowBehaviorTab()
{
  if (ImGui::BeginTabItem("Behavior")) {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults")) {
      Prefs::ResetBehaviorPreferences(gSettings);
      UpdatePreviewSettings(gSettings);
    }
    ImGui::Spacing();

    if (bool restore = gSettings.flags & Preferences::restore_last_session;
        Checkbox("Restore last session on startup", &restore))
    {
      gSettings.SetFlag(Preferences::restore_last_session, restore);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Preferred tile width: ");
    ImGui::SameLine();

    if (auto width = gSettings.preferred_tile_width;
        ImGui::DragInt("##PreferredTileWidth", &width, 1.0f, 1, 10'000))
    {
      gSettings.preferred_tile_width = width;
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile width when creating maps");
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Preferred tile height:");
    ImGui::SameLine();

    if (auto height = gSettings.preferred_tile_height;
        ImGui::DragInt("##PreferredTileHeight", &height, 1.0f, 1, 10'000))
    {
      gSettings.preferred_tile_height = height;
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile height when creating maps");
    }

    // TODO "RMB with stamp tool works as eraser"

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Command capacity:     ");
    ImGui::SameLine();

    if (auto capacity = static_cast<int>(gSettings.command_capacity);
        ImGui::DragInt("##CommandCapacityInput", &capacity, 1.0f, 10, 1'000))
    {
      gSettings.command_capacity = static_cast<usize>(capacity);
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The maximum amount of commands that will be stored on the undo stack");
    }

    ImGui::EndTabItem();
  }
}

void ShowAppearanceBar()
{
  if (ImGui::BeginTabItem("Appearance")) {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults")) {
      Prefs::ResetAppearancePreferences(gSettings);
      UpdatePreviewSettings(gSettings);
    }
    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Theme: ");
    ImGui::SameLine();

    if (ImGui::BeginCombo("##ThemeCombo", magic_enum::enum_name(gSettings.theme).data()))
    {
      for (auto&& [name, value] : themes) {
        if (ImGui::Selectable(name)) {
          gSettings.theme = value;
          ApplyTheme(ImGui::GetStyle(), value);
        }
      }

      ImGui::EndCombo();
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Viewport background color: ");
    ImGui::SameLine();

    if (auto array = ColorToArray(gSettings.viewport_background);
        ImGui::ColorEdit3("Viewport Background Color",
                          array.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                              ImGuiColorEditFlags_NoAlpha))
    {
      gSettings.viewport_background =
          cen::color::from_norm(array.at(0), array.at(1), array.at(2));
    }

    if (bool enabled = gSettings.flags & Preferences::window_border;
        ImGui::Checkbox("Window border", &enabled))
    {
      gSettings.SetFlag(Preferences::window_border, enabled);
      ImGui::GetStyle().WindowBorderSize = enabled ? 1.0f : 0.0f;
    }

    if (bool restoreLayout = gSettings.flags & Preferences::restore_layout;
        Checkbox("Restore layout",
                 &restoreLayout,
                 "Restore the previous layout of widgets at startup"))
    {
      gSettings.SetFlag(Preferences::restore_layout, restoreLayout);
    }

    ImGui::EndTabItem();
  }
}

void ShowExportTab()
{
  if (ImGui::BeginTabItem("Export")) {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults")) {
      Prefs::ResetExportPreferences(gSettings);
      UpdatePreviewSettings(gSettings);
    }
    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Preferred format: ");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##PreferredFormatCombo", gSettings.preferred_format.c_str())) {
      if (ImGui::MenuItem("YAML")) {
        gSettings.preferred_format = "YAML";
      }

      if (ImGui::MenuItem("JSON")) {
        gSettings.preferred_format = "JSON";
      }

      if (ImGui::MenuItem("XML (TMX)")) {
        gSettings.preferred_format = "XML";
      }

      ImGui::EndCombo();
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The save file format used if no file extension is specified when saving maps");
    }

    if (bool embedTilesets = gSettings.flags & Preferences::embed_tilesets;
        Checkbox("Embed tilesets", &embedTilesets, "Embed tileset data in map files"))
    {
      gSettings.SetFlag(Preferences::embed_tilesets, embedTilesets);
    }

    if (bool indentOutput = gSettings.flags & Preferences::indent_output;
        Checkbox("Indent output",
                 &indentOutput,
                 "Controls whether or not save files are indented"))
    {
      gSettings.SetFlag(Preferences::indent_output, indentOutput);
    }

    if (bool foldTileData = gSettings.flags & Preferences::fold_tile_data; Checkbox(
            "Fold tile data",
            &foldTileData,
            "Make tile layer data easier for humans to edit, at the expense of space"))
    {
      gSettings.SetFlag(Preferences::fold_tile_data, foldTileData);
    }

    ImGui::EndTabItem();
  }
}

void ApplySettings(entt::dispatcher& dispatcher)
{
  SetPreferences(gSettings);
  if (gSettings.command_capacity != gSnapshot.command_capacity) {
    dispatcher.enqueue<SetCommandCapacityEvent>(Prefs::GetCommandCapacity());
  }
}

}  // namespace

void UpdateSettingsDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{"Settings", gFlags, &gIsVisible}) {
    if (ImGui::BeginTabBar("SettingsTabBar")) {
      ShowBehaviorTab();
      ShowAppearanceBar();
      ShowExportTab();
      ImGui::EndTabBar();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("OK")) {
      ApplySettings(dispatcher);
      UpdatePreviewSettings(GetPreferences());
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      UpdatePreviewSettings(GetPreferences());
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Apply")) {
      ApplySettings(dispatcher);
      UpdatePreviewSettings(GetPreferences());
    }
  }
}

void OpenSettingsDialog()
{
  gIsVisible = true;
  gSnapshot = GetPreferences();
  gSettings = gSnapshot;
  ImGui::OpenPopup("Settings");
}

}  // namespace Tactile
