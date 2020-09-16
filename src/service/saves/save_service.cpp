#include "save_service.hpp"

#include <qfileinfo.h>

#include "save_tmx.hpp"

using namespace tactile::core;

namespace tactile::service {

void save(const QUrl& url, const map& map, const tileset_manager& tilesets)
{
  const QFileInfo info{url.fileName()};
  if (const auto suffix = info.suffix(); suffix == QStringLiteral(u"tmx")) {
    save_tmx(url, map, tilesets);

  } else if (suffix == QStringLiteral(u"json")) {
  } else {
    qWarning("Did not recognize save format!");
  }
}

}  // namespace tactile::service
