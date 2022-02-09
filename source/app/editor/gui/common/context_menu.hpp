#pragma once

#include <entt/entt.hpp>

#include "tactile.hpp"

namespace tactile {

class document_model;

class context_menu
{
 public:
  TACTILE_DEFAULT_COPY(context_menu)
  TACTILE_DEFAULT_MOVE(context_menu)

  explicit context_menu(const char* name);

  virtual ~context_menu() noexcept = default;

  void update(const document_model& model, entt::dispatcher& dispatcher);

  void show();

 protected:
  virtual void on_update([[maybe_unused]] const document_model& model,
                         [[maybe_unused]] entt::dispatcher& dispatcher)
  {}

 private:
  const char* mName{};
  bool mShow{};
};

}  // namespace tactile
