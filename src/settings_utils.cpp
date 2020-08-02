#include "settings_utils.hpp"

#include <QSettings>

namespace tactile {
namespace {

[[nodiscard]] Maybe<QVariant> get_value(const char* setting) noexcept
{
  if (setting) {
    QSettings settings;
    const QString key = setting;
    if (settings.contains(key)) {
      return settings.value(key);
    }
  }

  return nothing;
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

Maybe<QByteArray> settings_byte_array(const char* setting) noexcept
{
  const auto value = get_value(setting);
  if (value && value->canConvert(QVariant::ByteArray)) {
    return value->toByteArray();
  } else {
    return nothing;
  }
}

Maybe<bool> settings_bool(const char* setting) noexcept
{
  const auto value = get_value(setting);
  if (value && value->canConvert<bool>()) {
    return value->toBool();
  } else {
    return nothing;
  }
}

}  // namespace tactile
