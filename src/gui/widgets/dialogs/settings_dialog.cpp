#include "settings_dialog.hpp"

#include <imgui.h>

#include "events/change_command_capacity_event.hpp"
#include "gui/themes.hpp"
#include "gui/widgets/common/checkbox.hpp"
#include "gui/widgets/common/combo.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constexpr auto flags =
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

inline Preferences snapshot;  // The original settings when the dialog was opened
inline Preferences settings;  // The value of the settings in the GUI

void ShowGeneralTab(entt::dispatcher& dispatcher)
{
  if (ImGui::BeginTabItem("General"))
  {
    ImGui::Spacing();
    ImGui::Button("Restore Defaults");
    ImGui::Spacing();

    // TODO "Restore last session on startup"
    // TODO "RMB with stamp tool works as eraser"

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Command capacity:");
    ImGui::SameLine();

    if (auto capacity = static_cast<int>(settings.command_capacity);
        ImGui::DragInt("##CommandCapacityInput", &capacity, 1.0f, 10, 1'000))
    {
      settings.command_capacity = static_cast<usize>(capacity);
    }

    if (ImGui::IsItemHovered())
    {
      ImGui::SetTooltip(
          "The maximum amount of commands that will be stored on the undo stack.");
    }

    ImGui::EndTabItem();
  }
}

void ShowThemeBar()
{
  if (ImGui::BeginTabItem("Theme"))
  {
    ImGui::Spacing();
    ImGui::Button("Restore Defaults");
    ImGui::Spacing();

    if (auto index = GetThemeIndex(settings.theme);
        Combo("Theme:", theme_options, &index))
    {
      settings.theme = GetThemeFromIndex(index);
      ApplyTheme(ImGui::GetStyle(), settings.theme);
    }

    if (auto enabled = settings.window_border;
        ImGui::Checkbox("Window border", &enabled))
    {
      settings.window_border = enabled;
      ImGui::GetStyle().WindowBorderSize = enabled ? 1.0f : 0.0f;
    }

    if (auto restoreLayout = settings.restore_layout;
        Checkbox("Restore layout",
                 &restoreLayout,
                 "Restore the previous layout of widgets at startup."))
    {
      settings.restore_layout = restoreLayout;
    }

    ImGui::EndTabItem();
  }
}

void ShowExportTab()
{
  if (ImGui::BeginTabItem("Export"))
  {
    ImGui::Spacing();
    ImGui::Button("Restore Defaults");
    ImGui::Spacing();

    if (auto index = (settings.preferred_format == "JSON") ? 0 : 1;
        Combo("Preferred format:",
              "JSON\0TMX\0\0",
              &index,
              "The default save file format."))
    {
      settings.preferred_format = (index == 0) ? "JSON" : "TMX";
    }

    if (auto embedTilesets = settings.embed_tilesets;
        Checkbox("Embed tilesets",
                 &embedTilesets,
                 "Embed tileset data in map files."))
    {
      settings.embed_tilesets = embedTilesets;
    }

    if (auto humanReadableOutput = settings.human_readable_output;
        Checkbox("Human-readable output",
                 &humanReadableOutput,
                 "Make save files easier for humans to edit, at the cost of space."))
    {
      settings.human_readable_output = humanReadableOutput;
    }

    ImGui::EndTabItem();
  }
}

void ApplySettings(entt::dispatcher& dispatcher)
{
  SetPreferences(settings);
  if (settings.command_capacity != snapshot.command_capacity)
  {
    dispatcher.enqueue<ChangeCommandCapacityEvent>(Prefs::GetCommandCapacity());
  }
}

void RestorePreviewSettings()
{
  ApplyTheme(ImGui::GetStyle(), Prefs::GetTheme());
  ImGui::GetStyle().WindowBorderSize = Prefs::GetWindowBorder() ? 1.0f : 0.0f;
}

}  // namespace

void UpdateSettingsDialog(entt::dispatcher& dispatcher)
{
  if (ImGui::BeginPopupModal("Settings", nullptr, flags))
  {
    if (ImGui::BeginTabBar("SettingsTabBar"))
    {
      ShowGeneralTab(dispatcher);
      ShowThemeBar();
      ShowExportTab();
      ImGui::EndTabBar();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("OK"))
    {
      ApplySettings(dispatcher);
      RestorePreviewSettings();
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
      RestorePreviewSettings();
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Apply"))
    {
      ApplySettings(dispatcher);
      RestorePreviewSettings();
    }

    ImGui::EndPopup();
  }
}

void OpenSettingsDialog()
{
  snapshot = GetPreferences();
  settings = snapshot;
  ImGui::OpenPopup("Settings");
}

}  // namespace Tactile
