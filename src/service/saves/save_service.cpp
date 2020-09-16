#include "save_service.hpp"

#include <qdebug.h>
#include <qfileinfo.h>

#include "save_tmx.hpp"

using namespace tactile::core;

namespace tactile::service {

void save(const QString& path, const map& map, const tileset_manager& tilesets)
{
  const QFileInfo info{path};

  if (const auto suffix = info.suffix(); suffix == QStringLiteral(u"tmx")) {
    save_tmx(path, map, tilesets);

  } else if (suffix == QStringLiteral(u"json")) {
  } else {
    qWarning("Did not recognize save format!");
  }
}

}  // namespace tactile::service
