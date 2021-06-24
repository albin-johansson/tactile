#include "show_file_dialog.hpp"

#include "ImGuiFileDialog.h"

namespace tactile {

auto ShowFileDialog(const std::string& id,
                    const std::string& title,
                    const czstring filter,
                    const std::string& directory) -> bool
{
  auto* dialog = IGFD::FileDialog::Instance();
  dialog->OpenDialog(id, title, filter, directory);

  if (dialog->Display(id))
  {
    const auto ok = dialog->IsOk();
    dialog->Close();

    return ok;
  }

  return false;
}

auto GetFileDialogSelectedPath() -> std::filesystem::path
{
  auto* dialog = IGFD::FileDialog::Instance();
  return std::filesystem::path{dialog->GetFilePathName()};
}

}  // namespace tactile
