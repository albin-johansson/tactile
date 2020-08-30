#pragma once

#include <QByteArray>
#include <QPalette>

#include "setting.hpp"

namespace tactile::prefs {

namespace window {

[[nodiscard]] auto last_layout_geometry() -> setting<QByteArray>;

[[nodiscard]] auto last_layout_state() -> setting<QByteArray>;

}  // namespace window

namespace graphics {

[[nodiscard]] auto render_grid() -> setting<bool>;

[[nodiscard]] auto theme() -> setting<QPalette>;

[[nodiscard]] auto theme_name() -> setting<QString>;

}  // namespace graphics

/**
 * @brief Initializes the settings to their default values, if they are absent.
 *
 * @since 0.1.0
 */
void validate();

}  // namespace tactile::prefs
