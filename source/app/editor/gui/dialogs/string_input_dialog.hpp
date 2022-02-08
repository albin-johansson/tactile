#pragma once

#include <array>        // array
#include <string>       // string
#include <string_view>  // string_view

#include <entt/entt.hpp>

#include "dialog.hpp"
#include "tactile.hpp"

namespace tactile {

class AStringInputDialog : public dialog_base
{
 public:
  explicit AStringInputDialog(c_str title);

  void Show(std::string previous);

 protected:
  void on_update(const Model& model, entt::dispatcher& dispatcher) final;

  void SetInputHint(c_str hint);

  [[nodiscard]] auto is_current_input_valid(const Model& model) const -> bool final;

  [[nodiscard]] virtual auto Validate(const Model& model, std::string_view input) const
      -> bool = 0;

  [[nodiscard]] auto GetCurrentInput() const -> std::string_view;

  [[nodiscard]] auto GetPreviousString() const -> const std::string&;

 private:
  c_str mHint{};
  std::string mPrevious;
  std::array<char, 128> mBuffer{};
  bool mShouldAcquireFocus{};
};

}  // namespace tactile