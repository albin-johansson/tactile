#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include "editor/gui/dialogs/dialog.hpp"

namespace Tactile {

class ChangePropertyTypeDialog final : public ADialog {
 public:
  ChangePropertyTypeDialog();

  ~ChangePropertyTypeDialog() override = default;

  void Show(std::string name, PropertyType type);

 protected:
  void UpdateContents(const entt::registry& registry,
                      entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsCurrentInputValid(const entt::registry& registry) const
      -> bool override;

 private:
  PropertyType mCurrentType{PropertyType::String};
  Maybe<std::string> mPropertyName;
  Maybe<PropertyType> mPreviousType;
};

}  // namespace Tactile
