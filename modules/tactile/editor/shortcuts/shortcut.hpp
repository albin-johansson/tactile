#pragma once

#include <centurion.hpp>
#include <entt/entt.hpp>

#include "tactile_def.hpp"

namespace tactile {

class Model;
class WidgetManager;

class AShortcut {
 public:
  explicit AShortcut(const cen::scan_code key,
                     const cen::key_mod modifiers = cen::key_mod::none)
      : mKey{key}
      , mModifiers{modifiers}
  {}

  virtual ~AShortcut() noexcept = default;

  void Poll(const Model& model,
            const WidgetManager& widgets,
            const cen::keyboard_event& event,
            entt::dispatcher& dispatcher);

  virtual void Activate(entt::dispatcher& dispatcher) = 0;

  [[nodiscard]] virtual auto IsEnabled(
      [[maybe_unused]] const Model& model,
      [[maybe_unused]] const WidgetManager& widgets) const -> bool
  {
    return true;
  }

 private:
  cen::scan_code mKey;
  cen::key_mod mModifiers{cen::key_mod::none};
};

}  // namespace tactile
