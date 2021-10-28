#include "map_import_error_dialog.hpp"

#include <tactile-base/tactile_std.hpp>

#include <imgui.h>

#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/modal.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

inline Maybe<IO::ParseError> current_error;

}  // namespace

void UpdateMapImportErrorDialog()
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{TAC_ICON_ERROR " Map import error", flags}) {
    ImGui::TextUnformatted("Oops, failed to open the specified map!");
    ImGui::Text("Cause: %s", IO::GetCause(current_error.value()));

    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("OK")) {
      current_error.reset();
      ImGui::CloseCurrentPopup();
    }
  }
}

void OpenMapImportErrorDialog(const IO::ParseError error)
{
  current_error = error;
  ImGui::OpenPopup(TAC_ICON_ERROR " Map import error");
}

}  // namespace Tactile
