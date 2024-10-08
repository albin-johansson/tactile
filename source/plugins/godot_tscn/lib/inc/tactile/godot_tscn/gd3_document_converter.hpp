// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>  // size_t

#include "tactile/base/document/document_visitor.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/godot_tscn/api.hpp"
#include "tactile/godot_tscn/gd3_types.hpp"

namespace tactile::godot {

/**
 * A document visitor that produces intermediate Godot 3 scene representations.
 */
class TACTILE_GODOT_API Gd3DocumentConverter final : public IDocumentVisitor
{
 public:
  /**
   * Creates a document converter.
   *
   * \param options The write options to use.
   */
  explicit Gd3DocumentConverter(SaveFormatWriteOptions options);

  void set_ellipse_polygon_vertices(std::size_t count);

  [[nodiscard]]
  auto visit(const IComponentView& component) -> std::expected<void, std::error_code> override;

  [[nodiscard]]
  auto visit(const IMapView& map) -> std::expected<void, std::error_code> override;

  [[nodiscard]]
  auto visit(const ILayerView& layer) -> std::expected<void, std::error_code> override;

  [[nodiscard]]
  auto visit(const IObjectView& object) -> std::expected<void, std::error_code> override;

  [[nodiscard]]
  auto visit(const ITilesetView& tileset) -> std::expected<void, std::error_code> override;

  [[nodiscard]]
  auto visit(const ITileView& tile) -> std::expected<void, std::error_code> override;

  /**
   * Returns the generated Godot 3 map.
   *
   * \return
   * An intermediate representation of a Godot 3 map.
   */
  [[nodiscard]]
  auto get_map() const -> const Gd3Map&;

 private:
  SaveFormatWriteOptions m_options;
  Gd3Map m_map;
  std::size_t m_ellipse_polygon_vertices {};
};

}  // namespace tactile::godot
