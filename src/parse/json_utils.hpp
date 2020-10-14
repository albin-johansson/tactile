#pragma once

#include <QFileInfo>
#include <QJsonDocument>

namespace tactile::json {

[[nodiscard]] auto from_file(const QFileInfo& path) -> QJsonDocument;

}
