#pragma once

#include "editor/gui/dock_widget.hpp"

namespace tactile {

/**
 * \brief Used to manage the components associated with attribute contexts.
 *
 * \ingroup gui
 */
class component_dock final : public dock_widget
{
 public:
  component_dock();

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;

  void set_visible(bool visible) override;

  [[nodiscard]] auto is_visible() const -> bool override;
};

}  // namespace tactile
