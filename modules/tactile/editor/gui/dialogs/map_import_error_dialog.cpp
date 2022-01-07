#include "map_import_error_dialog.hpp"

#include <imgui.h>
#include <tactile_def.hpp>

#include "editor/gui/alignment.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {
namespace {

constexpr auto gFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

inline Maybe<IO::ParseError> gCurrentError;

}  // namespace

void UpdateMapImportErrorDialog()
{
  CenterNextWindowOnAppearance();
  if (Scoped::Modal modal{TAC_ICON_ERROR " Map import error", gFlags}; modal.IsOpen()) {
    ImGui::TextUnformatted("Oops, failed to open the specified map!");
    ImGui::Text("Cause: %s", IO::GetCause(gCurrentError.value()));

    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("OK")) {
      gCurrentError.reset();
      ImGui::CloseCurrentPopup();
    }
  }
}

void OpenMapImportErrorDialog(const IO::ParseError error)
{
  gCurrentError = error;
  ImGui::OpenPopup(TAC_ICON_ERROR " Map import error");
}

}  // namespace Tactile
