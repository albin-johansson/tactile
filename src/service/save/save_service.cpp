#include "save_service.hpp"

#include <QFileInfo>

#include "save_json.hpp"
#include "save_tmx.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

using namespace tactile::core;

namespace tactile::service {

void save(const QString& path, const core::map_document& document)
{
  const QFileInfo info{path};

  if (const auto suffix = info.suffix(); suffix == TACTILE_QSTRING(u"tmx")) {
    save_tmx(path, document);

  } else if (suffix == TACTILE_QSTRING(u"json")) {
    save_json(path, document);

  } else {
    throw tactile_error{"Did not recognize save format!"};
  }
}

}  // namespace tactile::service
