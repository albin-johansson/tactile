#include "shortcut.hpp"

namespace Tactile {
namespace {

[[nodiscard]] auto IsOnlyActive(const uint16 modifiers, const uint16 currentMask) noexcept
    -> bool
{
  if (modifiers == KMOD_NONE) {
    return !currentMask;
  }

  const auto hits = modifiers & currentMask;

  if (hits != modifiers) {
    return false; /* The specified modifiers were a combo that wasn't fully active */
  }
  else {
    const auto others = currentMask & ~hits;
    return hits && !others;
  }
}

}  // namespace

void AShortcut::Poll(const Model& model,
                     const WidgetManager& widgets,
                     const SDL_KeyboardEvent& event,
                     entt::dispatcher& dispatcher)
{
  if (IsEnabled(model, widgets)) {
    if (event.type == SDL_KEYDOWN && event.keysym.scancode == mKey &&
        IsOnlyActive(mModifiers, event.keysym.mod)) {
      Activate(dispatcher);
    }
  }
}

}  // namespace Tactile
