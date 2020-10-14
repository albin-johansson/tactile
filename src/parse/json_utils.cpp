#include "json_utils.hpp"

#include "tactile_error.hpp"

namespace tactile::json {

auto from_file(const QFileInfo& path) -> QJsonDocument
{
  QFile file{path.absoluteFilePath()};
  file.open(QFile::ReadOnly | QFile::Text);

  auto json = QJsonDocument::fromJson(file.readAll());

  if (json.isNull()) {
    file.close();
    throw tactile_error{"Failed to open JSON map file!"};
  } else {
    file.close();
  }

  return json;
}

void write_file(const QFileInfo& path, const QJsonDocument& document)
{
  QFile file{path.absoluteFilePath()};
  file.open(QFile::WriteOnly | QFile::Text);
  file.write(document.toJson());
  file.close();
}

}  // namespace tactile::json
