#pragma once

#include <QSettings>
#include <QString>
#include <optional>
#include <utility>

namespace tactile {

template <typename T>
class setting final {
 public:
  explicit setting(const QString& key)
  {
    const QSettings settings;
    Q_ASSERT(settings.status() == QSettings::NoError);

    const auto val = settings.value(key);
    if (!val.isNull() && val.isValid() && val.canConvert<T>()) {
      m_value.emplace(val.value<T>());
    }
  }

  auto operator*() const -> const T& { return *m_value; }

  auto operator->() const -> const T& { return *m_value; }

  [[nodiscard]] auto value_or(T&& fallback) const -> T
  {
    return m_value.value_or(std::forward<T>(fallback));
  }

  explicit operator bool() const noexcept { return m_value.has_value(); }

 private:
  std::optional<T> m_value;
};

namespace settings {

template <typename U>
void set(const QString& key, U&& value)
{
  QSettings settings;
  Q_ASSERT(settings.status() == QSettings::NoError);

  settings.setValue(key, std::forward<U>(value));
}

template <typename U>
void set_if_missing(const QString& key, U&& value)
{
  QSettings settings;
  Q_ASSERT(settings.status() == QSettings::NoError);

  if (!settings.contains(key)) {
    settings.setValue(key, std::forward<U>(value));
  }
}

}  // namespace settings
}  // namespace tactile
