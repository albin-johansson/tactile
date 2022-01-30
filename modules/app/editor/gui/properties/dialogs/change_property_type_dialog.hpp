#pragma once

#include <string>  // string

#include "core/attribute_value.hpp"
#include "editor/gui/dialogs/dialog.hpp"
#include "tactile_def.hpp"

namespace tactile {

class ChangePropertyTypeDialog final : public ADialog {
 public:
  ChangePropertyTypeDialog();

  ~ChangePropertyTypeDialog() override = default;

  void Show(std::string name, attribute_type type);

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsCurrentInputValid(const Model& model) const -> bool override;

 private:
  attribute_type mCurrentType{attribute_type::string};
  maybe<std::string> mPropertyName;
  maybe<attribute_type> mPreviousType;
};

}  // namespace tactile
