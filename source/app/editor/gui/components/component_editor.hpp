#pragma once

#include <memory>  // unique_ptr

#include <entt/entt.hpp>

#include "core/attribute.hpp"
#include "editor/gui/dialogs/dialog.hpp"
#include "tactile.hpp"

namespace tactile {

class Model;

class component_editor final : public ADialog
{
 public:
  TACTILE_DEFAULT_MOVE(component_editor)
  TACTILE_DELETE_COPY(component_editor)

  component_editor();

  ~component_editor() noexcept override;

  void Open(const Model& model);

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

 private:
  struct component_editor_data;
  std::unique_ptr<component_editor_data> mData;

  void show_component_combo_popup(const entt::registry& registry,
                                  entt::dispatcher& dispatcher);

  void show_component_attributes(const entt::registry& registry,
                                 entt::dispatcher& dispatcher,
                                 component_id id);

  void show_component_attribute(entt::dispatcher& dispatcher,
                                component_id id,
                                const std::string& name,
                                const attribute_value& value);
};

}  // namespace tactile
