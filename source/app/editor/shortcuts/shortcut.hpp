#pragma once

#include <centurion.hpp>
#include <entt/entt.hpp>

#include "tactile.hpp"

namespace tactile {

class document_model;
class widget_manager;

class AShortcut
{
 public:
  explicit AShortcut(const cen::scan_code key,
                     const cen::key_mod modifiers = cen::key_mod::none)
      : mKey{key}
      , mModifiers{modifiers}
  {}

  virtual ~AShortcut() noexcept = default;

  void Poll(const document_model& model,
            const widget_manager& widgets,
            const cen::keyboard_event& event,
            entt::dispatcher& dispatcher);

  virtual void Activate(entt::dispatcher& dispatcher) = 0;

  [[nodiscard]] virtual auto IsEnabled(
      [[maybe_unused]] const document_model& model,
      [[maybe_unused]] const widget_manager& widgets) const -> bool
  {
    return true;
  }

 private:
  cen::scan_code mKey;
  cen::key_mod mModifiers{cen::key_mod::none};
};

}  // namespace tactile
