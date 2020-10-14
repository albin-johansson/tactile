#include "xml_utils.hpp"

#include <QFile>

namespace tactile::xml {

auto to_elem(const QDomNode& node) -> QDomElement
{
  if (node.isElement()) {
    return node.toElement();
  } else {
    throw tactile_error{"Failed to convert XML node to element!"};
  }
}

auto from_file(const QFileInfo& path) -> QDomDocument
{
  QDomDocument document{};

  QFile file{path.absoluteFilePath()};
  file.open(QFile::ReadOnly | QFile::Text);

  QString error;
  if (const auto success = document.setContent(file.readAll(), &error);
      !success) {
    file.close();
    qDebug() << error;
    throw tactile_error{"Failed to parse TMX file!"};
  } else {
    file.close();
  }

  return document;
}

}  // namespace tactile::xml
