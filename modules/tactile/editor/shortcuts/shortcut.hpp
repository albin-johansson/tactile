#pragma once

#include <tactile_def.hpp>

#include <SDL.h>
#include <centurion.hpp>  // scan_code, key_mod, keyboard_event
#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

class Model;

class AShortcut
{
 public:
  explicit AShortcut(const cen::scan_code key, const uint16 modifiers = KMOD_NONE)
      : mKey{key}
      , mModifiers{modifiers}
  {}

  virtual ~AShortcut() noexcept = default;

  void Poll(const Model& model,
            const SDL_KeyboardEvent& event,
            entt::dispatcher& dispatcher);

  virtual void Activate(entt::dispatcher& dispatcher) = 0;

  [[nodiscard]] virtual auto IsEnabled([[maybe_unused]] const Model& model) const -> bool
  {
    return true;
  }

 private:
  cen::scan_code mKey;
  uint16 mModifiers{KMOD_NONE};
};

}  // namespace Tactile
