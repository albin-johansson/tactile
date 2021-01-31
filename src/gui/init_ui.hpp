#pragma once

#include <QWidget>

namespace tactile::gui {

// clang-format off
template <typename T, typename U>
concept ui_class = requires(T t, U* self)
{
  { t.setupUi(self) };
};

/**
 * \brief Initializes the external UI for a class.
 *
 * \note The returned pointer must be deleted.
 *
 * \pre `self` cannot be null.
 *
 * \tparam T the type of the UI-class, e.g. `Ui::foo`.
 * \tparam Self the type of the class that hosts the UI-member.
 *
 * \param self should be the `this` pointer.
 *
 * \return a pointer to the created UI-class instance, must be deleted.
 */
template <typename T, typename Self> requires ui_class<T, Self>
[[nodiscard]] auto init_ui(Self* self) -> T*
{
  Q_ASSERT(self);

  auto* ui = new T{};
  ui->setupUi(self);

  return ui;
}

// clang-format on

}  // namespace tactile::gui
