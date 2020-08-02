#include "settings_utils.hpp"

#include <QSettings>
#include <optional>

namespace tactile {
namespace {

// FIXME these functionsa are not noexcept

[[nodiscard]] auto get_value(const char* setting) noexcept
    -> std::optional<QVariant>
{
  if (setting) {
    QSettings settings;
    const QString key = setting;
    if (settings.contains(key)) {
      return settings.value(key);
    }
  }

  return std::nullopt;
}

}  // namespace

void set_if_absent(const char* setting, const QVariant& value) noexcept
{
  if (setting) {
    QSettings settings;
    const QString key = setting;
    if (!settings.contains(key)) {
      settings.setValue(key, value);
    }
  }
}

auto settings_byte_array(const char* setting) noexcept
    -> std::optional<QByteArray>
{
  const auto value = get_value(setting);
  if (value && value->canConvert(QVariant::ByteArray)) {
    return value->toByteArray();
  } else {
    return std::nullopt;
  }
}

auto settings_bool(const char* setting) noexcept -> std::optional<bool>
{
  const auto value = get_value(setting);
  if (value && value->canConvert<bool>()) {
    return value->toBool();
  } else {
    return std::nullopt;
  }
}

}  // namespace tactile
