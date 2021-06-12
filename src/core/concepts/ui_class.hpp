#pragma once

namespace tactile {

// clang-format off

template <typename Ui, typename Self>
concept UiClass = requires (Ui ui, Self* self)
{
  { ui.setupUi(self) };
};

// clang-format on

}  // namespace tactile
