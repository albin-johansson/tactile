#include "json_utils.hpp"

#include <QSaveFile>

#include "preferences.hpp"

namespace tactile::json {

auto from_file(const QFileInfo& path) -> maybe<QJsonDocument>
{
  QFile file{path.absoluteFilePath()};
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    return std::nullopt;
  }

  auto json = QJsonDocument::fromJson(file.readAll());

  if (json.isNull()) {
    file.close();
    return std::nullopt;
  } else {
    file.close();
  }

  return json;
}

auto write_file(const QFileInfo& path, const QJsonDocument& document) -> bool
{
  QSaveFile file{path.absoluteFilePath()};

  file.setDirectWriteFallback(true);
  file.open(QFile::WriteOnly | QFile::Text);

  constexpr auto def = prefs::saves::readable_output_def();
  const auto format = prefs::saves::readable_output().value_or(def)
                          ? QJsonDocument::JsonFormat::Indented
                          : QJsonDocument::JsonFormat::Compact;

  if (const auto result = file.write(document.toJson(format)); result == -1) {
    file.cancelWriting();
    return false;
  } else {
    file.commit();
    return true;
  }
}

}  // namespace tactile::json
