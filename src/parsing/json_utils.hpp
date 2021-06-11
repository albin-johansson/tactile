#pragma once

#include <QFileInfo>      // QFileInfo
#include <QJsonDocument>  // QJsonDocument

#include "maybe.hpp"

namespace tactile {

/**
 * \brief Opens and returns the JSON file at the specified path.
 *
 * \param path the file path of the JSON file.
 *
 * \return a JSON document; `std::nullopt` if something went wrong.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto ReadJson(const QFileInfo& path) -> maybe<QJsonDocument>;

/**
 * \brief Writes a JSON document to the specified file path.
 *
 * \param path the destination file path of the JSON file.
 * \param document the JSON document that will be saved.
 *
 * \return `true` if the file was successfully written to; `false` otherwise.
 *
 * \since 0.1.0
 */
auto WriteJson(const QFileInfo& path, const QJsonDocument& document) -> bool;

}  // namespace tactile
