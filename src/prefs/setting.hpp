#pragma once

#include <QSettings>
#include <QString>
#include <optional>
#include <utility>

namespace tactile::prefs {

template <typename T>
class setting final
{
 public:
  explicit setting(QString key) : m_key{std::move(key)}
  {
    const QSettings settings;
    Q_ASSERT(settings.status() == QSettings::NoError);

    const auto val = settings.value(m_key);
    if (!val.isNull() && val.isValid() && val.canConvert<T>()) {
      m_value.emplace(val.value<T>());
    }
  }

  void set(const T& value)
  {
    QSettings settings;
    Q_ASSERT(settings.status() == QSettings::NoError);

    settings.setValue(m_key, value);
    m_value = value;
  }

  void set_if_missing(const T& value)
  {
    QSettings settings;
    Q_ASSERT(settings.status() == QSettings::NoError);

    if (!settings.contains(m_key)) {
      settings.setValue(m_key, value);
      m_value = value;
    }
  }

  auto operator*() const -> const T&
  {
    return *m_value;
  }

  auto operator->() const -> const T&
  {
    return *m_value;
  }

  [[nodiscard]] auto value_or(T&& fallback) const -> T
  {
    return m_value.value_or(std::forward<T>(fallback));
  }

  explicit operator bool() const noexcept
  {
    return m_value.has_value();
  }

 private:
  std::optional<T> m_value;
  QString m_key;
};

}  // namespace tactile::prefs
