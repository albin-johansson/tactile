#pragma once

#include <array>        // array
#include <string>       // string
#include <string_view>  // string_view

#include <tactile_def.hpp>

#include <entt/entt.hpp>

#include "dialog.hpp"

namespace Tactile {

class ARenameDialog : public ADialog {
 public:
  explicit ARenameDialog(CStr title);

  void Show(std::string oldName);

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) final;

  [[nodiscard]] auto IsCurrentInputValid(const Model& model) const -> bool final;

  [[nodiscard]] virtual auto Validate(const Model& model, std::string_view input) const
      -> bool = 0;

  [[nodiscard]] auto GetCurrentInput() const -> std::string_view;

  [[nodiscard]] auto GetPreviousName() const -> const std::string&;

 private:
  std::string mOldName;
  std::array<char, 128> mNameBuffer{};
};

}  // namespace Tactile