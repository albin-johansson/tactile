#pragma once

#include <QPalette>
#include <QString>

#include "tactile_types.hpp"

namespace tactile::prefs::theme {

void reset();

void set_theme(const QString& name);

[[nodiscard]] auto from_name(const QString& name) -> QPalette;

[[nodiscard]] auto parse(const QByteArray& name) -> QPalette;

[[nodiscard]] auto get_dark() -> const QPalette&;

[[nodiscard]] auto get_light() -> const QPalette&;

[[nodiscard]] auto get_atom_one_dark() -> const QPalette&;

[[nodiscard]] auto get_default() -> const QPalette&;

[[nodiscard]] auto get_default_name() -> QStringView;

}  // namespace tactile::prefs::theme
