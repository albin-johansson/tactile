#pragma once

#include <entt/entt.hpp>

#include "tactile.hpp"

namespace tactile {

class Model;

class ADialog {
 public:
  explicit ADialog(c_str title);

  virtual ~ADialog() noexcept = default;

  void Update(const Model& model, entt::dispatcher& dispatcher);

 protected:
  /* Dialog specific GUI code is implemented with this function */
  virtual void UpdateContents(const Model& model, entt::dispatcher& dispatcher) = 0;

  /* Invoked when the "Cancel" button is pressed */
  virtual void OnCancel() {}

  /* Invoked when the "OK" (accept) button is pressed */
  virtual void OnAccept([[maybe_unused]] entt::dispatcher& dispatcher) {}

  /* Invoked when the "Apply" (secondary accept) button is pressed */
  virtual void OnApply([[maybe_unused]] entt::dispatcher& dispatcher) {}

  /* Indicates whether the current input state is acceptable */
  [[nodiscard]] virtual auto IsCurrentInputValid(
      [[maybe_unused]] const Model& model) const -> bool
  {
    return true;
  }

  void Show();

  void SetAcceptButtonLabel(c_str label);

  void SetApplyButtonLabel(c_str label);

  void SetCloseButtonLabel(c_str label);

 private:
  c_str mTitle;
  c_str mAcceptButtonLabel{"OK"};
  c_str mApplyButtonLabel{}; /* By default, not shown (when null) */
  c_str mCloseButtonLabel{"Cancel"};
  bool mShow{};
};

}  // namespace tactile
