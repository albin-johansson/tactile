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
[[nodiscard]] auto OpenUsing(const QString& file, parse::ParseError& error)
    -> core::MapDocument*
{
  parse::MapParser<T> parser{file};
  error = parser.GetError();
  if (parser)
  {
    return parser.MakeDocument();
  }
  else
  {
    return nullptr;
  }
}

}  // namespace

auto OpenMapDocument(const QString& path, parse::ParseError& error)
    -> core::MapDocument*
{
  const QFileInfo info{path};
  const auto suffix = info.suffix();

  if (suffix == TACTILE_QSTRING(u"json"))
  {
    return OpenUsing<parse::JsonEngine>(path, error);
  }
  else if (suffix == TACTILE_QSTRING(u"tmx"))
  {
    return OpenUsing<parse::XmlEngine>(path, error);
  }
  else
  {
    throw TactileError{"Did not recognize map format to open!"};
  }
}

}  // namespace tactile
