#pragma once

#include <array>        // array
#include <string>       // string
#include <string_view>  // string_view

#include <tactile_def.hpp>

#include <entt/entt.hpp>

#include "dialog.hpp"

namespace Tactile {

class AStringInputDialog : public ADialog {
 public:
  explicit AStringInputDialog(CStr title);

  void Show(std::string previous);

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) final;

  void SetInputHint(CStr hint);

  [[nodiscard]] auto IsCurrentInputValid(const Model& model) const -> bool final;

  [[nodiscard]] virtual auto Validate(const Model& model, std::string_view input) const
      -> bool = 0;

  [[nodiscard]] auto GetCurrentInput() const -> std::string_view;

  [[nodiscard]] auto GetPreviousString() const -> const std::string&;

 private:
  CStr mHint{};
  std::string mPrevious;
  std::array<char, 128> mBuffer{};
};

}  // namespace Tactile