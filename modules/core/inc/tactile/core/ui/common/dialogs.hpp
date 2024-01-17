// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::ui {

using DialogFlagsBits = uint8;

/**
 * Provides option flags for dialogs.
 */
enum DialogFlags : DialogFlagsBits {
  kDialogFlagOpenBit = 1u << 0u,          ///< If set, the dialog will be opened.
  kDialogFlagInputIsValidBit = 1u << 1u,  ///< Indicates that the current input is valid.
};

/**
 * Represents different possible dialog actions.
 */
enum class DialogAction : int8 {
  kNone,    ///< No action was taken.
  kCancel,  ///< The "cancel" action was triggered.
  kApply,   ///< The "apply" action was triggered.
  kAccept,  ///< The "accept" action was triggered.
};

/**
 * Provides options for scoped dialogs.
 */
struct ScopedDialogOptions final {
  const char* title;                ///< The dialog title label.
  const char* close_button_label;   ///< The "close" button label, if any.
  const char* apply_button_label;   ///< The "apply" button label, if any.
  const char* accept_button_label;  ///< The "accept" button label, if any.
  bool* is_open;                    ///< If set, controls the dialog visibility.
  DialogFlagsBits flags;            ///< The dialog options.
};

/**
 * RAII utility for showing dialogs.
 */
class TACTILE_CORE_API ScopedDialog final {
 public:
  TACTILE_DELETE_COPY(ScopedDialog);
  TACTILE_DELETE_MOVE(ScopedDialog);

  /**
   * Begins recording a dialog.
   *
   * \param      options The dialog options.
   * \param[out] action  The outcome of the dialog.
   */
  explicit ScopedDialog(const ScopedDialogOptions& options,
                        DialogAction* action = nullptr);

  /**
   * Ends the dialog recording.
   */
  ~ScopedDialog() noexcept;

  /**
   * Indicates whether the dialog is open.
   *
   * \return
   *    True if the dialog is open; false otherwise.
   */
  [[nodiscard]]
  auto is_open() const -> bool;

 private:
  ScopedDialogOptions mOptions;
  DialogAction* mAction;
  bool mIsOpen;
};

}  // namespace tactile::ui
