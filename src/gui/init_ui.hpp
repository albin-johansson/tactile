#pragma once

#include <QWidget>

namespace tactile::gui {

// clang-format off
template <typename T, typename U>
concept ui_class = requires(T t, U* self) {
  { t.setupUi(self) };
};

template <typename T, typename Self> requires ui_class<T, Self>
[[nodiscard]] auto init_ui(Self* self) -> T*
{
  auto* ui = new T{};
  ui->setupUi(self);
  return ui;
}

// clang-format on

}  // namespace tactile::gui
