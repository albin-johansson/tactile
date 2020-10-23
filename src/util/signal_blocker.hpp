#pragma once

#include <QObject>

namespace tactile {

/**
 * @class signal_blocker
 *
 * @brief Used to disable all signals from a `QObject` during the enclosing
 * scope.
 *
 * @since 0.1.0
 *
 * @headerfile signal_blocker.hpp
 */
class signal_blocker final
{
 public:
  // clang-format off
  /**
   * @brief Creates a signal blocker instance.
   *
   * @pre 'object' cannot be null.
   *
   * @param object the object to disable the signals for.
   *
   * @since 0.1.0
   */
  [[nodiscard("Must stay alive during the entire scope!")]]
  explicit signal_blocker(QObject* object) noexcept : m_object{object}
  {
    Q_ASSERT(m_object);
    m_object->blockSignals(true);
  }
  // clang-format on

  /**
   * @brief Restores the signals from the associated object.
   *
   * @since 0.1.0
   */
  ~signal_blocker() noexcept
  {
    m_object->blockSignals(false);
  }

 private:
  QObject* m_object{};
};

}  // namespace tactile
