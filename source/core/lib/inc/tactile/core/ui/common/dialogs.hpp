// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"

namespace tactile::ui {

/**
 * Represents possible actions related to dialogs.
 */
enum class DialogStatus : std::uint8_t
{
  /** Nothing happened. */
  kIdle,

  /** The dialog was closed via the "close" button. */
  kClosed,

  /** The "apply" button was pressed. */
  kApplied,

  /** The dialog was closed via the "accept" button. */
  kAccepted,
};

/**
 * Pushes a dialog button section to the widget stack.
 *
 * \details
 * All dialogs feature a bottom button group that at least features buttons for
 * cancelling and accepting the current changes. This function provides an easy
 * way to provide one such group. It's encouraged to use verbs suitable for the
 * context of the dialog in button labels. For example, instead of just using
 * "OK" as the accept button label, prefer terms such as "Create" or "Change"
 * since those are much more descriptive.
 *
 * \param close      The label used by the "close" button.
 * \param accept     The label used by the "accept" button.
 * \param apply      The label used by the "apply" button, can be null.
 * \param can_accept Controls whether the "accept" button is enabled.
 * \param can_apply  Controls whether the "apply" button is enabled.
 *
 * \return
 * The current dialog status.
 */
[[nodiscard]]
auto push_dialog_control_buttons(const char* close,
                                 const char* accept,
                                 const char* apply = nullptr,
                                 bool can_accept = true,
                                 bool can_apply = true) -> DialogStatus;

}  // namespace tactile::ui
