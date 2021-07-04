#include "file_dialog.hpp"

#include "ImGuiFileDialog.h"

namespace Tactile {
namespace {

inline constexpr auto flags = ImGuiFileDialogFlags_DontShowHiddenFiles |
                              ImGuiFileDialogFlags_DisableCreateDirectoryButton;

}  // namespace

auto FileDialog(const std::string& id,
                const std::string& title,
                czstring filter) -> FileDialogResult
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

auto GetFileDialogSelectedPath() -> std::filesystem::path
{
  auto* dialog = IGFD::FileDialog::Instance();
  return std::filesystem::path{dialog->GetFilePathName()};
}

}  // namespace Tactile
