#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace tactile {

class document_model;

class Toolbar final
{
 public:
  void Update(const document_model& model, entt::dispatcher& dispatcher);

  void SetVisible(bool visible);

  [[nodiscard]] auto IsVisible() const noexcept -> bool { return mVisible; }

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  bool mVisible{true};
  bool mHasFocus{};
};

}  // namespace tactile
