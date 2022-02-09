#pragma once

#include <entt/entt.hpp>

#include "tactile.hpp"

namespace tactile {

class document_model;

/**
 * \brief Serves as the base implementation of all modal dialogs.
 */
class dialog_base
{
 public:
  TACTILE_DEFAULT_COPY(dialog_base)
  TACTILE_DEFAULT_MOVE(dialog_base)

  /**
   * \brief Creates a dialog.
   *
   * \param title the non-null title of the dialog.
   *
   * \throws tactile_error if the title is null.
   */
  explicit dialog_base(c_str title);

  virtual ~dialog_base() noexcept = default;

  /**
   * \brief Updates the state of the dialog.
   *
   * \param model the associated model.
   * \param dispatcher the event dispatcher used.
   */
  void update(const document_model& model, entt::dispatcher& dispatcher);

 protected:
  /**
   * \brief Updates dialog specific components.
   *
   * \details Do not provide components such as accept and close buttons in this function.
   *
   * \param model the associated model.
   * \param dispatcher the event dispatcher used.
   */
  virtual void on_update(const document_model& model, entt::dispatcher& dispatcher) = 0;

  /**
   * \brief Invoked when the "Cancel" button is pressed.
   */
  virtual void on_cancel() {}

  /**
   * \brief Invoked when the "OK" (accept) button is pressed.
   *
   * \param dispatcher the event dispatcher used.
   */
  virtual void on_accept([[maybe_unused]] entt::dispatcher& dispatcher) {}

  /**
   * \brief Invoked when the "Apply" (secondary accept) button is pressed.
   *
   * \param dispatcher the event dispatcher used.
   */
  virtual void on_apply([[maybe_unused]] entt::dispatcher& dispatcher) {}

  /**
   * \brief Indicates whether the current input state is considered acceptable.
   *
   * \param model the associated model.
   *
   * \return `true` if the state of the input is acceptable; `false` otherwise.
   */
  [[nodiscard]] virtual auto is_current_input_valid(
      [[maybe_unused]] const document_model& model) const -> bool
  {
    return true;
  }

  /**
   * \brief Makes the dialog visible.
   *
   * \details This is meant to be used by derived classes to actually display the dialog.
   * This function is protected since not all dialogs supports being shown with no initial
   * arguments.
   */
  void make_visible();

  /**
   * \brief Includes an "Apply" button.
   *
   * \details By default, the apply button is not shown.
   *
   * \see on_apply()
   */
  void use_apply_button();

  /**
   * \brief Sets the label of the "accept" button.
   *
   * \details By default, the accept button uses "OK" as its label. However, it is often
   * more intuitive to use more descriptive verbs as the label, e.g. "Create", "Rename",
   * etc.
   *
   * \param label the label text;
   *              a null pointer may be used to indicate that the button will be hidden.
   */
  void set_accept_button_label(c_str label);

  /**
   * \brief Sets the label of the "close" button.
   *
   * \details By default, the accept button uses "Cancel" as its label.
   *
   * \param label the label text;
   *              a null pointer may be used to indicate that the button will be hidden.
   */
  void set_close_button_label(c_str label);

 private:
  c_str mTitle{};
  c_str mAcceptButtonLabel{"OK"};
  c_str mCloseButtonLabel{"Cancel"};
  bool mUseApplyButton{};
  bool mShow{};
};

}  // namespace tactile
