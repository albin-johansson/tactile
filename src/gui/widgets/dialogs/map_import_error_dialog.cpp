#include "map_import_error_dialog.hpp"

#include <imgui.h>

#include "aliases/maybe.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/alignment.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

inline Maybe<IO::ParseError> current_error;

}  // namespace

void UpdateMapImportErrorDialog()
{
  CenterNextWindowOnAppearance();
  if (ImGui::BeginPopupModal(TAC_ICON_ERROR " Map import error", nullptr, flags)) {
    ImGui::TextUnformatted("Oops, failed to open the specified map!");
    ImGui::Text("Cause: %s", IO::GetCause(current_error.value()));

    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("OK")) {
      current_error.reset();
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void OpenMapImportErrorDialog(const IO::ParseError error)
{
  current_error = error;
  ImGui::OpenPopup(TAC_ICON_ERROR " Map import error");
}

}  // namespace Tactile
