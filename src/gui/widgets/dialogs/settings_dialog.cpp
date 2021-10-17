#include "settings_dialog.hpp"

#include <imgui.h>

#include <magic_enum.hpp>  // enum_name

#include "core/utils/color_utils.hpp"
#include "gui/events/command_events.hpp"
#include "gui/themes.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/checkbox.hpp"
#include "gui/widgets/common/combo.hpp"
#include "gui/widgets/common/modal.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit bool is_visible = false;

inline Preferences snapshot;  // The original settings when the dialog was opened
inline Preferences settings;  // The value of the settings in the GUI

void UpdatePreviewSettings(const Preferences& prefs)
{
  ApplyTheme(ImGui::GetStyle(), prefs.theme);
  ImGui::GetStyle().WindowBorderSize = prefs.window_border ? 1.0f : 0.0f;
}

void ShowBehaviorTab()
{
  if (ImGui::BeginTabItem("Behavior")) {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults")) {
      Prefs::ResetBehaviorPreferences(settings);
      UpdatePreviewSettings(settings);
    }
    ImGui::Spacing();

    if (auto restore = settings.restore_last_session;
        Checkbox("Restore last session on startup", &restore))
    {
      settings.restore_last_session = restore;
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Preferred tile width: ");
    ImGui::SameLine();

    if (auto width = settings.preferred_tile_width;
        ImGui::DragInt("##PreferredTileWidth", &width, 1.0f, 1, 10'000))
    {
      settings.preferred_tile_width = width;
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile width when creating maps");
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Preferred tile height:");
    ImGui::SameLine();

    if (auto height = settings.preferred_tile_height;
        ImGui::DragInt("##PreferredTileHeight", &height, 1.0f, 1, 10'000))
    {
      settings.preferred_tile_height = height;
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile height when creating maps");
    }

    // TODO "RMB with stamp tool works as eraser"

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Command capacity:     ");
    ImGui::SameLine();

    if (auto capacity = static_cast<int>(settings.command_capacity);
        ImGui::DragInt("##CommandCapacityInput", &capacity, 1.0f, 10, 1'000))
    {
      settings.command_capacity = static_cast<usize>(capacity);
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
      Prefs::ResetAppearancePreferences(settings);
      UpdatePreviewSettings(settings);
    }
    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Theme: ");
    ImGui::SameLine();

    if (ImGui::BeginCombo("##ThemeCombo", magic_enum::enum_name(settings.theme).data())) {
      for (auto&& [name, value] : themes) {
        if (ImGui::Selectable(name)) {
          settings.theme = value;
          ApplyTheme(ImGui::GetStyle(), value);
        }
      }

      ImGui::EndCombo();
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Viewport background color: ");
    ImGui::SameLine();

    if (auto array = ColorToArray(settings.viewport_background);
        ImGui::ColorEdit3("Viewport Background Color",
                          array.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                              ImGuiColorEditFlags_NoAlpha))
    {
      settings.viewport_background =
          cen::color::from_norm(array.at(0), array.at(1), array.at(2));
    }

    if (auto enabled = settings.window_border; ImGui::Checkbox("Window border", &enabled))
    {
      settings.window_border = enabled;
      ImGui::GetStyle().WindowBorderSize = enabled ? 1.0f : 0.0f;
    }

    if (auto restoreLayout = settings.restore_layout;
        Checkbox("Restore layout",
                 &restoreLayout,
                 "Restore the previous layout of widgets at startup"))
    {
      settings.restore_layout = restoreLayout;
    }

    ImGui::EndTabItem();
  }
}

void ShowExportTab()
{
  if (ImGui::BeginTabItem("Export")) {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults")) {
      Prefs::ResetExportPreferences(settings);
      UpdatePreviewSettings(settings);
    }
    ImGui::Spacing();

    ImGui::TextUnformatted("Preferred format: ");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##PreferredFormatCombo", settings.preferred_format.c_str())) {
      if (ImGui::MenuItem("YAML")) {
        settings.preferred_format = "YAML";
      }

      if (ImGui::MenuItem("JSON")) {
        settings.preferred_format = "JSON";
      }

      if (ImGui::MenuItem("XML (TMX)")) {
        settings.preferred_format = "XML";
      }

      ImGui::EndCombo();
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The save file format used if no file extension is specified when saving maps");
    }

    if (auto embedTilesets = settings.embed_tilesets;
        Checkbox("Embed tilesets", &embedTilesets, "Embed tileset data in map files"))
    {
      settings.embed_tilesets = embedTilesets;
    }

    if (auto humanReadableOutput = settings.human_readable_output;
        Checkbox("Human-readable output",
                 &humanReadableOutput,
                 "Make save files easier for humans to edit, at the expense of space"))
    {
      settings.human_readable_output = humanReadableOutput;
    }

    ImGui::EndTabItem();
  }
}

void ApplySettings(entt::dispatcher& dispatcher)
{
  SetPreferences(settings);
  if (settings.command_capacity != snapshot.command_capacity) {
    dispatcher.enqueue<SetCommandCapacityEvent>(Prefs::GetCommandCapacity());
  }
}

}  // namespace

void UpdateSettingsDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{"Settings", flags, &is_visible}) {
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
  is_visible = true;
  snapshot = GetPreferences();
  settings = snapshot;
  ImGui::OpenPopup("Settings");
}

}  // namespace Tactile
