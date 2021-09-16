#pragma once

#include <centurion.hpp>  // scan_code, key_mod, keyboard_event
#include <entt.hpp>       // dispatcher

namespace Tactile {

class Model;

class AShortcut
{
 public:
  AShortcut(const cen::scan_code key, const cen::key_mod modifiers = cen::key_mod::none)
      : mKey{key}
      , mModifiers{modifiers}
  {}

  virtual ~AShortcut() noexcept = default;

  void Poll(const Model& model,
            const cen::keyboard_event& event,
            entt::dispatcher& dispatcher);

  virtual void Activate(entt::dispatcher& dispatcher) = 0;

  [[nodiscard]] virtual auto IsEnabled(const Model& model) const -> bool
  {
    return true;
  }

 private:
  cen::scan_code mKey;
  cen::key_mod mModifiers;
};

}  // namespace Tactile
