#include "tactile/core/ui/common/dialogs.hpp"

#include <imgui.h>

#include "tactile/core/ui/common/alignment.hpp"
#include "tactile/core/ui/common/buttons.hpp"
#include "tactile/foundation/debug/assert.hpp"

namespace tactile::core {
namespace {

[[nodiscard]]
auto _begin_dialog(const ScopedDialogOptions& options) -> bool
{
  TACTILE_ASSERT(options.title != nullptr);

  if (options.flags & kDialogFlagOpenBit) {
    ImGui::OpenPopup(options.title);
  }

  center_next_window_on_appearance();

  const ImGuiWindowFlags popup_flags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

  return ImGui::BeginPopupModal(options.title, options.is_open, popup_flags);
}

[[nodiscard]]
auto _end_dialog(const ScopedDialogOptions& options) noexcept -> DialogAction
{
  const bool input_is_valid = options.flags & kDialogFlagInputIsValidBit;
  auto dialog_action = DialogAction::kNone;

  ImGui::Spacing();

  // "Close" button
  if (options.close_button_label && show_button(options.close_button_label)) {
    dialog_action = DialogAction::kCancel;
    ImGui::CloseCurrentPopup();
  }

  // "Accept" button
  if (options.accept_button_label) {
    if (options.close_button_label) {
      ImGui::SameLine();
    }

    if (show_button(options.accept_button_label, nullptr, input_is_valid)) {
      dialog_action = DialogAction::kAccept;
      ImGui::CloseCurrentPopup();
    }
  }

  // "Apply" button
  if (options.apply_button_label) {
    if (options.close_button_label || options.accept_button_label) {
      ImGui::SameLine();
    }

    if (show_button(options.apply_button_label, nullptr, input_is_valid)) {
      dialog_action = DialogAction::kApply;
    }
  }

  ImGui::EndPopup();
  return dialog_action;
}

}  // namespace

ScopedDialog::ScopedDialog(const ScopedDialogOptions& options, DialogAction* action)
  : mOptions {options},
    mAction {action},
    mIsOpen {_begin_dialog(mOptions)}
{}

ScopedDialog::~ScopedDialog() noexcept
{
  if (mIsOpen) {
    const auto dialog_action = _end_dialog(mOptions);
    if (mAction) {
      *mAction = dialog_action;
    }
  }
}

auto ScopedDialog::is_open() const -> bool
{
  return mIsOpen;
}

}  // namespace tactile::core