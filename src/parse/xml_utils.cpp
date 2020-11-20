#include "xml_utils.hpp"

#include <QSaveFile>

namespace tactile::xml {

auto to_elem(const QDomNode& node) -> QDomElement
{
  if (node.isElement()) {
    return node.toElement();
  } else {
    throw tactile_error{"Failed to convert XML node to element!"};
  }
}

auto from_file(const QFileInfo& path) -> std::optional<QDomDocument>
{
  QDomDocument document{};

  QFile file{path.absoluteFilePath()};
  file.open(QFile::ReadOnly | QFile::Text);

  QString error;
  if (const auto success = document.setContent(file.readAll(), &error);
      !success) {
    file.close();
    return std::nullopt;
  } else {
    file.close();
  }

  return document;
}

void write_file(const QFileInfo& path, const QDomDocument& document)
{
  QSaveFile file{path.absoluteFilePath()};

  file.setDirectWriteFallback(true);
  file.open(QFile::WriteOnly | QFile::Text);
  if (const auto result = file.write(document.toByteArray()); result == -1) {
    file.cancelWriting();
  } else {
    file.commit();
  }
}

}  // namespace tactile::xml
