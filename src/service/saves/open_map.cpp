#include "open_map.hpp"

#include <qfileinfo.h>

#include <qjsondocument.h>

namespace tactile::service {

auto open_map(const QString& path) -> open_map_result
{
  const QFileInfo info{path};

  // TODO

  if (const auto suffix = info.suffix(); suffix == QStringLiteral(u"json")) {
  } else if (suffix == QStringLiteral(u"tmx")) {
  }

  return open_map_result();
}

}  // namespace tactile::service
