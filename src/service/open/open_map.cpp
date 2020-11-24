#include "open_map.hpp"

#include <QFileInfo>

#include "json_engine.hpp"
#include "map_parser.hpp"
#include "tactile_error.hpp"
#include "tiled_json_parser.hpp"
#include "tiled_tmx_parser.hpp"
#include "xml_engine.hpp"

namespace tactile::service {
namespace {

template <typename T>
auto open_using(const QFileInfo& file, parse_error* error)
    -> core::map_document*
{
  tmx::map_parser<T> parser{file};
  *error = parser.error_code();
  if (parser) {
    return parser.take_document();
  } else {
    return nullptr;
  }
}

}  // namespace

auto open_map(const QString& path, parse_error* error) -> core::map_document*
{
  const QFileInfo info{path};
  const auto suffix = info.suffix();
  if (suffix == QStringLiteral(u"json")) {
    return open_using<tmx::json_engine>(info, error);

  } else if (suffix == QStringLiteral(u"tmx")) {
    return open_using<tmx::xml_engine>(info, error);

  } else {
    throw tactile_error{"Did not recognize map format to open!"};
  }
}

}  // namespace tactile::service
