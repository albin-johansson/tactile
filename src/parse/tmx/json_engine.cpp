#include "json_engine.hpp"

#include "json_utils.hpp"

namespace tactile::tmx {
namespace {

[[nodiscard]] auto index_to_position(const int index, const int nCols)
    -> core::position
{
  return core::position{row_t{index / nCols}, col_t{index % nCols}};
}

}  // namespace

auto json_engine::root(const document_type& document) -> object_type
{
  return object_type{document.object()};
}

auto json_engine::from_file(const QFileInfo& path) -> maybe<document_type>
{
  return json::from_file(path);
}

auto json_engine::add_tiles(core::tile_layer& layer,
                            const object_type& element,
                            parse_error& error) -> bool
{
  const auto nCols = layer.col_count().get();
  const auto data = element->value(u"data").toArray();

  int index{0};
  for (const auto value : data) {
    const tile_id id{value.toInt(-1)};

    if (id == -1_t) {
      error = parse_error::layer_could_not_parse_tile;
      return false;
    } else {
      layer.set_tile(index_to_position(index, nCols), id);
    }

    ++index;
  }

  return true;
}

}  // namespace tactile::tmx
