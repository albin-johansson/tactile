#pragma once

#include <memory>  // unique_ptr

#include <entt/entt.hpp>

#include "dialogs/rename_layer_dialog.hpp"
#include "editor/gui/dock_widget.hpp"
#include "tactile.hpp"

namespace tactile {

class document_model;

class layer_dock final : public dock_widget
{
 public:
  layer_dock();

  ~layer_dock() noexcept override;

  void show_rename_layer_dialog(layer_id id);

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;

  void set_visible(bool visible) override;

  [[nodiscard]] auto is_visible() const -> bool override;

 private:
  struct layer_dock_data;
  std::unique_ptr<layer_dock_data> mData;

  void update_buttons(const document_model& model,
                      const entt::registry& registry,
                      entt::dispatcher& dispatcher);
};

}  // namespace tactile
