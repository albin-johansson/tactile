#pragma once

#include <string>  // string

#include "editor/gui/dialogs/dialog.hpp"
#include "tactile_def.hpp"

namespace tactile {

class ChangePropertyTypeDialog final : public ADialog {
 public:
  ChangePropertyTypeDialog();

  ~ChangePropertyTypeDialog() override = default;

  void Show(std::string name, PropertyType type);

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsCurrentInputValid(const Model& model) const -> bool override;

 private:
  PropertyType mCurrentType{PropertyType::String};
  Maybe<std::string> mPropertyName;
  Maybe<PropertyType> mPreviousType;
};

}  // namespace tactile
