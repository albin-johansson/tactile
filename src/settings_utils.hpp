#pragma once
#include "maybe.hpp"

class QVariant;
class QByteArray;

namespace tactile {

/**
 * Sets the value of a setting associated with the specified key if no
 * previous setting exists. This method has no effect if the supplied pointer
 * is null.
 *
 * @param setting the name of the key of the setting to set if absent.
 * @param value the value to associate with the key.
 * @since 0.1.0
 */
void set_if_absent(const char* setting, const QVariant& value) noexcept;

/**
 * Returns a byte array associated with the specified key from the settings if
 * it exists. This method has no effect if the supplied pointer is null.
 *
 * @param setting the name of the key associated with the byte array, can
 * safely be null.
 * @return a byte array if it exists; nothing otherwise.
 * @since 0.1.0
 */
[[nodiscard]] Maybe<QByteArray> settings_byte_array(
    const char* setting) noexcept;

/**
 * Returns the boolean value from the settings that is associated with the
 * specified key. This method has no effect if the supplied pointer is null.
 *
 * @param setting the name of the key associated with the boolean value, can
 * safely be null.
 * @return the value of the setting, if it exists; nothing otherwise.
 * @since 0.1.0
 */
[[nodiscard]] Maybe<bool> settings_bool(const char* setting) noexcept;

}  // namespace tactile
