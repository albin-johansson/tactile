#include "file_dialog.hpp"

#include "ImGuiFileDialog.h"

namespace Tactile {

auto FileDialog(const std::string& id,
                const std::string& title,
                const CStr filter,
                const ImGuiFileDialogFlags flags) -> FileDialogResult
{
  auto* dialog = IGFD::FileDialog::Instance();
  dialog->OpenDialog(id, title, filter, std::string{}, 1, nullptr, flags);

  if (dialog->Display(id))
  {
    const auto ok = dialog->IsOk();
    dialog->Close();
    return ok ? FileDialogResult::Success : FileDialogResult::Close;
  }

  return FileDialogResult::Idle;
}

auto FileDialogExport(const std::string& id,
                      const std::string& title,
                      const CStr filter) -> FileDialogResult
{
  constexpr auto flags = ImGuiFileDialogFlags_DontShowHiddenFiles |
                         ImGuiFileDialogFlags_DisableCreateDirectoryButton |
                         ImGuiFileDialogFlags_ConfirmOverwrite;
  return FileDialog(id, title, filter, flags);
}

auto FileDialogImport(const std::string& id,
                      const std::string& title,
                      const CStr filter) -> FileDialogResult
{
  constexpr auto flags = ImGuiFileDialogFlags_DontShowHiddenFiles |
                         ImGuiFileDialogFlags_DisableCreateDirectoryButton;
  return FileDialog(id, title, filter, flags);
}

auto GetFileDialogSelectedPath() -> std::filesystem::path
{
  auto* dialog = IGFD::FileDialog::Instance();
  return std::filesystem::path{dialog->GetFilePathName()};
}

}  // namespace Tactile
