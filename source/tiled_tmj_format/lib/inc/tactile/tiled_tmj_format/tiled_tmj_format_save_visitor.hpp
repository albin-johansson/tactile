// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <nlohmann/json.hpp>

#include "tactile/base/document/document_visitor.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj_format/api.hpp"

namespace tactile {

class IMetaView;

class TACTILE_TMJ_FORMAT_API TiledTmjFormatSaveVisitor final : public IDocumentVisitor
{
 public:
  using JSON = nlohmann::json;


  [[nodiscard]]
  auto visit(const IMapView& map) -> Result<void> override;

  [[nodiscard]]
  auto visit(const ITilesetView& tileset) -> Result<void> override;

  [[nodiscard]]
  auto visit(const ITileView& tile) -> Result<void> override;

  [[nodiscard]]
  auto visit(const ILayerView& layer) -> Result<void> override;

  [[nodiscard]]
  auto visit(const IObjectView& object) -> Result<void> override;

  [[nodiscard]]
  auto visit(const IComponentView& component) -> Result<void> override;

  void set_options(SaveFormatWriteOptions options);

 private:
  SaveFormatWriteOptions mOptions {};
};

}  // namespace tactile
