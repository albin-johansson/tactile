#include "open_map_document.hpp"

#include <QFileInfo>  // QFileInfo

#include "json_engine.hpp"
#include "map_parser.hpp"
#include "tactile_error.hpp"
#include "xml_engine.hpp"

namespace tactile {
namespace {

template <typename T>
[[nodiscard]] auto OpenUsing(const QString& file, ParseError& error)
    -> core::MapDocument*
{
  MapParser<T> parser{file};
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

auto OpenMapDocument(const QString& path, ParseError& error)
    -> core::MapDocument*
{
  const QFileInfo info{path};
  const auto suffix = info.suffix();

  if (suffix == QStringLiteral(u"json"))
  {
    return OpenUsing<JsonEngine>(path, error);
  }
  else if (suffix == QStringLiteral(u"tmx"))
  {
    return OpenUsing<XmlEngine>(path, error);
  }
  else
  {
    throw TactileError{"Did not recognize map format to open!"};
  }
}

}  // namespace tactile
