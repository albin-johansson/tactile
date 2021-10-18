#include "convert_document_to_ir.hpp"

#include "core/components/layer_tree_node.hpp"
#include "core/map.hpp"
#include "editor/document.hpp"

namespace Tactile {

auto ConvertDocumentToIR(const Document& document) -> IO::MapData
{
  const auto& registry = document.registry;
  const auto& map = registry.ctx<Map>();

  IO::MapData data;
  data.absolute_path = document.path;
  data.next_layer_id = map.next_layer_id;
  data.next_object_id = map.next_object_id;
  data.tile_width = map.tile_width;
  data.tile_height = map.tile_height;

  return data;
}

}  // namespace Tactile
