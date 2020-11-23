#pragma once

#include <QSettings>
#include <QString>
#include <concepts>  // invocable, same_as
#include <utility>   // forward

#include "maybe.hpp"

namespace tactile::prefs {

// clang-format off
template <typename T>
concept negatable = requires(T t)
{
  { !t } -> std::convertible_to<T>;
};
// clang-format on

/**
 * \class setting
 *
 * \brief Represents a persistent setting.
 *
 * \tparam T the type of the value associated with the setting.
 *
 * \since 0.1.0
 *
 * \headerfile setting.hpp
 */
template <typename T>
class setting final
{
 public:
  /**
   * \brief Creates a setting based on the supplied key.
   *
   * \param key the key associated with the desired setting.
   *
   * \since 0.1.0
   */
  explicit setting(QString key) : m_key{std::move(key)}
  {
    const QSettings settings;
    Q_ASSERT(settings.status() == QSettings::NoError);

    const auto val = settings.value(m_key);
    if (!val.isNull() && val.isValid() && val.canConvert<T>()) {
      m_value.emplace(val.value<T>());
    }
  }

  /**
   * \brief Sets the value of the setting.
   *
   * \param value the new value of the setting.
   *
   * \since 0.1.0
   */
  void set(const T& value)
  {
    QSettings settings;
    Q_ASSERT(settings.status() == QSettings::NoError);

    settings.setValue(m_key, value);
    m_value = value;
  }

  /**
   * \brief Sets the value of the setting, if there is no current value.
   *
   * \param value the new value of the setting.
   *
   * \since 0.1.0
   */
  void set_if_missing(const T& value)
  {
    QSettings settings;
    Q_ASSERT(settings.status() == QSettings::NoError);

    if (!settings.contains(m_key)) {
      settings.setValue(m_key, value);
      m_value = value;
    }
  }

  /**
   * \brief Invokes a function object with the current value of the setting, if
   * there is one.
   *
   * \details This function was inspired by the fact that it can be a little
   * difficult to differentiate the conversion to `bool`, used to check for a
   * valid value of a setting, from obtaining the actual value of the setting.
   * This is especially noticeable with boolean settings.
   *
   * \tparam U the type of the function object.
   *
   * \param callable the function object that will be invoked if there is a
   * value associated with the setting.
   *
   * \since 0.1.0
   */
  template <std::invocable<const T&> U>
  void with(U&& callable)
  {
    if (m_value) {
      callable(*m_value);
    }
  }

  /**
   * \brief Toggles the value of the setting.
   *
   * \note This is only available when the underlying setting type is negatable.
   *
   * \since 0.1.0
   */
  void toggle() requires negatable<T>
  {
    if (m_value) {
      set(!*m_value);
    }
  }

  /**
   * \brief Returns a reference to the value of the setting.
   *
   * \return a reference to the value of the setting.
   *
   * \since 0.1.0
   */
  auto operator*() const -> const T&
  {
    return *m_value;
  }

  /**
   * \brief Returns a pointer to the value of the setting.
   *
   * \return a pointer to the value of the setting.
   *
   * \since 0.1.0
   */
  auto operator->() const -> const T*
  {
    return m_value.operator->();
  }

  [[nodiscard]] auto value() const -> const T&
  {
    return m_value.value();
  }

  /**
   * \brief Returns the current value of the setting, or the default value if
   * there is no current value.
   *
   * \param fallback the default value that is used if there is no current
   * value.
   *
   * \return the current value of the setting or `fallback`.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto value_or(const T& fallback) const -> T
  {
    return m_value.value_or(fallback);
  }

  /**
   * \brief Indicates whether or not the settings holds a valid value.
   *
   * \return `true` if the settings has a value; `false` otherwise.
   *
   * \since 0.1.0
   */
  explicit operator bool() const noexcept
  {
    return m_value.has_value();
  }

 private:
  maybe<T> m_value;
  QString m_key;
};

}  // namespace tactile::prefs
