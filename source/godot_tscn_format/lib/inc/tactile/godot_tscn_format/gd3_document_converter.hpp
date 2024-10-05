// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/document/document_visitor.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/runtime.hpp"
#include "tactile/godot_tscn_format/api.hpp"
#include "tactile/godot_tscn_format/gd3_types.hpp"

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
   * \param runtime The associated runtime, cannot be null.
   * \param options The write options to use.
   */
  Gd3DocumentConverter(IRuntime* runtime, SaveFormatWriteOptions options);

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
  IRuntime* m_runtime;
  SaveFormatWriteOptions m_options;
  Gd3Map m_map;
};

}  // namespace tactile::godot
