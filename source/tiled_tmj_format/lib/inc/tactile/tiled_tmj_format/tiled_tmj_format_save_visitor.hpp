// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <nlohmann/json.hpp>

#include "tactile/base/document/document_visitor.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj_format/api.hpp"

namespace tactile {

class IMetaView;

class TACTILE_TMJ_FORMAT_API TiledTmjFormatSaveVisitor final
  : public IDocumentVisitor
{
 public:
  using JSON = nlohmann::json;

  void visit(const IMapView& map) override;

  void visit(const ITilesetView& tileset) override;

  void visit(const ITileView& tile) override;

  void visit(const ILayerView& layer) override;

  void visit(const IObjectView& object) override;

  void visit(const IComponentView& component) override;

  void set_options(SaveFormatWriteOptions options);

 private:
  SaveFormatWriteOptions mOptions {};
};

}  // namespace tactile
