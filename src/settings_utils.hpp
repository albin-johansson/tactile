#pragma once

#include <optional>

#include "tactile_types.hpp"

class QVariant;
class QByteArray;

namespace tactile {

/**
 * @brief Sets the value of a setting associated with the specified key if no
 * previous setting exists.
 *
 * @note This method has no effect if the supplied pointer is null.
 *
 * @param setting the name of the key of the setting to set if absent.
 * @param value the value to associate with the key.
 *
 * @since 0.1.0
 */
void set_if_absent(czstring setting, const QVariant& value) noexcept;

/**
 * @brief Returns a byte array associated with the specified key from the
 * settings if it exists.
 *
 * @note This method has no effect if the supplied pointer is null.
 *
 * @param setting the name of the key associated with the byte array, can
 * safely be null.
 *
 * @return a byte array if it exists; `std::nullopt` otherwise.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto settings_byte_array(czstring setting) noexcept
    -> std::optional<QByteArray>;

/**
 * @brief Returns the boolean value from the settings that is associated with
 * the specified key.
 *
 * @note This method has no effect if the supplied pointer is null.
 *
 * @param setting the name of the key associated with the boolean value, can
 * safely be null.
 *
 * @return the value of the setting, if it exists; `std::nullopt` otherwise.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto settings_bool(czstring setting) noexcept
    -> std::optional<bool>;

}  // namespace tactile
