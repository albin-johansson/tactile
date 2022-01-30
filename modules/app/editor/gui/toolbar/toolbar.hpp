#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace tactile {

class Model;

class Toolbar final {
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

  void SetVisible(bool visible);

  [[nodiscard]] auto IsVisible() const noexcept -> bool { return mVisible; }

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  bool mVisible{true};
  bool mHasFocus{};
};

}  // namespace tactile
