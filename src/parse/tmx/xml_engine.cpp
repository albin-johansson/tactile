#include "xml_engine.hpp"

namespace tactile::tmx {

auto xml_engine::root(const document_type& document) -> object_type
{
  return object_type{document.documentElement()};
}

auto xml_engine::from_file(const QFileInfo& path) -> maybe<document_type>
{
  return xml::from_file(path);
}

auto xml_engine::add_tiles(core::layer& layer,
                           const object_type& element,
                           parse_error& error) -> bool
{
  const auto data = element->firstChildElement(TACTILE_QSTRING(u"data"));
  const auto tiles = data.text().split(u',');
  const auto nCols = layer.col_count().get();

  int index{0};
  for (const auto& value : tiles) {
    bool ok{};
    const tile_id id{value.toInt(&ok)};

    if (!ok) {
      error = parse_error::layer_could_not_parse_tile;
      return false;
    }

    const core::position pos{row_t{index / nCols}, col_t{index % nCols}};
    layer.set_tile(pos, id);
    ++index;
  }

  return true;
}

}  // namespace tactile::tmx
