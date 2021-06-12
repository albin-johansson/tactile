#include "xml_utils.hpp"

#include <QSaveFile>  // QSaveFile

namespace tactile {

auto ReadXml(const QFileInfo& path) -> Maybe<QDomDocument>
{
  QDomDocument document;

  QFile file{path.absoluteFilePath()};
  file.open(QFile::ReadOnly | QFile::Text);

  QString error;
  if (const auto success = document.setContent(file.readAll(), &error);
      !success)
  {
    file.close();
    return nothing;
  }
  else
  {
    file.close();
  }

  return document;
}

void WriteXml(const QFileInfo& path, const QDomDocument& document)
{
  QSaveFile file{path.absoluteFilePath()};

  file.setDirectWriteFallback(true);
  file.open(QFile::WriteOnly | QFile::Text);
  if (const auto result = file.write(document.toByteArray()); result == -1)
  {
    file.cancelWriting();
  }
  else
  {
    file.commit();
  }
}

}  // namespace tactile
