#include "open_map_document.hpp"

#include <QFileInfo>  // QFileInfo

#include "json_engine.hpp"
#include "map_parser.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "xml_engine.hpp"

namespace tactile {
namespace {

template <typename T>
auto open_using(const QString& file, parse::parse_error& error)
    -> core::map_document*
{
  parse::map_parser<T> parser{file};
  error = parser.error_code();
  if (parser)
  {
    return parser.make_document();
  }
  else
  {
    return nullptr;
  }
}

}  // namespace

auto open_map_document(const QString& path, parse::parse_error& error)
    -> core::map_document*
{
  const QFileInfo info{path};
  const auto suffix = info.suffix();

  if (suffix == TACTILE_QSTRING(u"json"))
  {
    return open_using<parse::json_engine>(path, error);
  }
  else if (suffix == TACTILE_QSTRING(u"tmx"))
  {
    return open_using<parse::xml_engine>(path, error);
  }
  else
  {
    throw tactile_error{"Did not recognize map format to open!"};
  }
}

}  // namespace tactile
