#pragma once

#include <QFileInfo>
#include <QJsonDocument>

#include "maybe.hpp"

namespace tactile::json {

[[nodiscard]] auto from_file(const QFileInfo& path) -> maybe<QJsonDocument>;

auto write_file(const QFileInfo& path, const QJsonDocument& document) -> bool;

}  // namespace tactile::json
