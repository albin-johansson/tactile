#include "map_import_error_dialog.hpp"

#include <imgui.h>

#include "aliases/czstring.hpp"
#include "aliases/maybe.hpp"
#include "gui/icons.hpp"

namespace Tactile {
namespace {

inline Maybe<IO::ParseError> current_error;

}  // namespace

void UpdateMapImportErrorDialog()
{
  constexpr auto flags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  if (ImGui::BeginPopupModal(ICON_FA_EXCLAMATION_TRIANGLE " Map import error",
                             nullptr,
                             flags))
  {
    ImGui::TextUnformatted("Oops, failed to open the specified map!");
    ImGui::Text("Cause: %s", IO::GetCause(current_error.value()));

    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("OK"))
    {
      current_error.reset();
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void OpenMapImportErrorDialog(const IO::ParseError error)
{
  current_error = error;
  ImGui::OpenPopup(ICON_FA_EXCLAMATION_TRIANGLE " Map import error");
}

}  // namespace Tactile
