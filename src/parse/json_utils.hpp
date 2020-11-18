#pragma once

#include <QFileInfo>
#include <QJsonDocument>
#include <optional>  // optional

namespace tactile::json {

[[nodiscard]] auto from_file(const QFileInfo& path)
    -> std::optional<QJsonDocument>;

auto write_file(const QFileInfo& path, const QJsonDocument& document) -> bool;

}  // namespace tactile::json
