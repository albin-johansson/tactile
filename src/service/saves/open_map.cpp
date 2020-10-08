#include "open_map.hpp"

#include <qfileinfo.h>

#include <qjsondocument.h>

namespace tactile::service {

auto open_map(const QString& path) -> core::map_document*
{
  const QFileInfo info{path};

  // TODO

  if (const auto suffix = info.suffix(); suffix == QStringLiteral(u"json")) {
  } else if (suffix == QStringLiteral(u"tmx")) {
  }

  return nullptr;
}

}  // namespace tactile::service
