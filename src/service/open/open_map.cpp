#include "open_map.hpp"

#include <QFileInfo>

#include "open_json.hpp"
#include "tactile_error.hpp"

namespace tactile::service {

auto open_map(const QString& path) -> core::map_document*
{
  const QFileInfo info{path};

  if (const auto suffix = info.suffix(); suffix == QStringLiteral(u"json")) {
    return open_json_map(info);
  } else if (suffix == QStringLiteral(u"tmx")) {
    // TODO
    throw tactile_error{"TMX parsing not implemented..."};
  } else {
    throw tactile_error{"Did not recognize map format to open!"};
  }
}

}  // namespace tactile::service
