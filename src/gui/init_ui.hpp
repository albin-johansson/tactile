#pragma once

#include <QWidget>  // QWidget

#include "smart_pointers.hpp"
#include "ui_class.hpp"

namespace tactile {

// clang-format off

/**
 * \brief Initializes the external UI for a class.
 *
 * \pre `self` cannot be null.
 *
 * \tparam T the type of the UI-class, e.g. `Ui::foo`.
 * \tparam Self the type of the class that hosts the UI-member.
 *
 * \param self should be the `this` pointer.
 *
 * \return the created UI-class instance.
 */
template <typename T, typename Self> requires UiClass<T, Self>
[[nodiscard]] auto InitUi(Self* self) -> Unique<T>
{
  Q_ASSERT(self);

  auto ui = std::make_unique<T>();
  ui->setupUi(self);

  return ui;
}

// clang-format on

}  // namespace tactile
