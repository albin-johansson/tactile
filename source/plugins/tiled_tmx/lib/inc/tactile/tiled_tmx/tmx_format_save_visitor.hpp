#pragma once

#include <cstddef>        // size_t
#include <string>         // string
#include <unordered_map>  // unordered_map
#include <vector>         // vector

#include <pugixml.hpp>

#include "tactile/base/document/document_visitor.hpp"
#include "tactile/base/document/tileset_view.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/runtime/runtime.hpp"
#include "tactile/tiled_tmx/api.hpp"

namespace tactile::tiled_tmx {

struct TmxTilesetDocument final
{
  std::string source;
  pugi::xml_document document;
};

class TACTILE_TILED_TMX_API TmxFormatSaveVisitor final : public IDocumentVisitor
{
 public:
  TmxFormatSaveVisitor(IRuntime* runtime, SaveFormatWriteOptions options);

  [[nodiscard]]
  auto visit(const IComponentView& component) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto visit(const IMapView& map) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto visit(const ILayerView& layer) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto visit(const IObjectView& object) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto visit(const ITilesetView& tileset) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto visit(const ITileView& tile) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto get_map_xml_document() const -> const pugi::xml_document&;

  [[nodiscard]]
  auto get_tileset_xml_documents() const -> const std::vector<TmxTilesetDocument>&;

 private:
  IRuntime* m_runtime;
  SaveFormatWriteOptions m_options;
  pugi::xml_document m_map_document;
  pugi::xml_node m_map_node;
  std::vector<pugi::xml_node> m_layer_nodes;
  std::vector<TmxTilesetDocument> m_tileset_documents;
  std::unordered_map<TileID, pugi::xml_node> m_tileset_nodes;

  [[nodiscard]]
  auto _get_tile_node(const ITilesetView& tileset, TileIndex tile_index) -> pugi::xml_node;
};

}  // namespace tactile::tiled_tmx
