#pragma once

#include <QSettings>  // QSettings
#include <QString>    // QString
#include <concepts>   // invocable, same_as
#include <utility>    // forward

#include "maybe.hpp"

namespace tactile {

// clang-format off

template <typename T>
concept IsNegatable = requires (T t)
{
  { !t } -> std::convertible_to<T>;
};

// clang-format on

/**
 * \class Setting
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
class Setting final
{
 public:
  /**
   * \brief Creates a setting based on the supplied key.
   *
   * \param key the key associated with the desired setting.
   *
   * \since 0.1.0
   */
  explicit Setting(QString key) : mKey{std::move(key)}
  {
    const QSettings settings;
    Q_ASSERT(settings.status() == QSettings::NoError);

    const auto val = settings.value(mKey);
    if (!val.isNull() && val.isValid() && val.canConvert<T>())
    {
      mValue.emplace(val.value<T>());
    }
  }

  /**
   * \brief Sets the value of the setting.
   *
   * \param value the new value of the setting.
   *
   * \since 0.1.0
   */
  void Set(const T& value)
  {
    QSettings settings;
    Q_ASSERT(settings.status() == QSettings::NoError);

    settings.setValue(mKey, value);
    mValue = value;
  }

  auto operator=(const T& value) -> Setting&
  {
    Set(value);
    return *this;
  }

  /**
   * \brief Sets the value of the setting, if there is no current value.
   *
   * \param value the new value of the setting.
   *
   * \since 0.1.0
   */
  void SetIfMissing(const T& value)
  {
    QSettings settings;
    Q_ASSERT(settings.status() == QSettings::NoError);

    if (!settings.contains(mKey))
    {
      settings.setValue(mKey, value);
      mValue = value;
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
  void With(U&& callable)
  {
    if (mValue)
    {
      callable(*mValue);
    }
  }

  /**
   * \brief Toggles the value of the setting.
   *
   * \note This is only available when the underlying setting type is negatable.
   *
   * \since 0.1.0
   */
  void Toggle() requires IsNegatable<T>
  {
    if (mValue)
    {
      set(!*mValue);
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
    return *mValue;
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
    return mValue.operator->();
  }

  [[nodiscard]] auto Value() const -> const T&
  {
    return mValue.value();
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
  [[nodiscard]] auto ValueOr(const T& fallback) const -> T
  {
    return mValue.value_or(fallback);
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
    return mValue.has_value();
  }

 private:
  maybe<T> mValue;
  QString mKey;
};

}  // namespace tactile
