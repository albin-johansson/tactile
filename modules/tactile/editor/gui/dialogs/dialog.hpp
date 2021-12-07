#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>

namespace Tactile {

class ADialog {
 public:
  explicit ADialog(CStr title);

  virtual ~ADialog() = default;

  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

 protected:
  /* Dialog specific GUI code is implemented with this function */
  virtual void UpdateContents(const entt::registry& registry,
                              entt::dispatcher& dispatcher) = 0;

  /* Invoked when the "Cancel" button is pressed */
  virtual void OnCancel() {}

  /* Invoked when the "OK" (accept) button is pressed */
  virtual void OnAccept([[maybe_unused]] entt::dispatcher& dispatcher) {}

  /* Invoked when the "Apply" (secondary accept) button is pressed */
  virtual void OnApply([[maybe_unused]] entt::dispatcher& dispatcher) {}

  /* Indicates whether the current input state is acceptable */
  [[nodiscard]] virtual auto IsCurrentInputValid(
      [[maybe_unused]] const entt::registry& registry) const -> bool
  {
    return true;
  }

  void Show();

  void SetAcceptButtonLabel(CStr label);

  void SetApplyButtonLabel(CStr label);

 private:
  CStr mTitle;
  CStr mAcceptButtonLabel{"OK"};
  CStr mApplyButtonLabel{}; /* By default, not shown (when null) */
  bool mShow{};
};

}  // namespace Tactile
