// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/test_util/document_view_mocks.hpp"

#include <utility>  // move

#include "tactile/base/io/tile_io.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"

namespace tactile::test {

MetaViewMock::MetaViewMock(ir::Metadata meta) :
  mMeta {std::move(meta)}
{
  using testing::Return;

  ON_CALL(*this, get_name).WillByDefault([this]() -> std::string_view { return mMeta.name; });

  ON_CALL(*this, get_property)
      .WillByDefault(
          [this](const std::size_t index) -> std::pair<const std::string&, const Attribute&> {
            const auto& property = mMeta.properties.at(index);
            return {property.name, property.value};
          });

  ON_CALL(*this, property_count).WillByDefault(testing::Return(mMeta.properties.size()));
}

ObjectViewMock::ObjectViewMock(ir::Object object,
                               std::variant<const ILayerView*, const ITileView*> parent) :
  mObject {std::move(object)},
  mParent {parent},
  mMeta {mObject.meta}
{
  using testing::Return;
  using testing::ReturnRef;

  ON_CALL(*this, accept).WillByDefault([this](IDocumentVisitor& visitor) {
    EXPECT_TRUE(visitor.visit(*this).has_value());
    return std::expected<void, std::error_code> {};
  });

  ON_CALL(*this, get_parent_layer).WillByDefault([this] {
    return std::holds_alternative<const ILayerView*>(mParent)
               ? std::get<const ILayerView*>(mParent)
               : nullptr;
  });

  ON_CALL(*this, get_parent_tile).WillByDefault([this] {
    return std::holds_alternative<const ITileView*>(mParent)
               ? std::get<const ITileView*>(mParent)
               : nullptr;
  });

  ON_CALL(*this, get_type).WillByDefault(Return(mObject.type));
  ON_CALL(*this, get_id).WillByDefault(Return(mObject.id));
  ON_CALL(*this, get_position).WillByDefault(Return(mObject.position));
  ON_CALL(*this, get_size).WillByDefault(Return(mObject.size));
  ON_CALL(*this, get_tag).WillByDefault(Return(mObject.tag));
  ON_CALL(*this, is_visible).WillByDefault(Return(mObject.visible));

  ON_CALL(*this, get_meta).WillByDefault(ReturnRef(mMeta));
}

TileViewMock::TileViewMock(const ITilesetView* parent_tileset, ir::Tile tile) :
  mParentTileset {parent_tileset},
  mTile {std::move(tile)},
  mMeta {mTile.meta}
{
  using testing::Return;
  using testing::ReturnRef;

  ON_CALL(*this, accept).WillByDefault([this](IDocumentVisitor& visitor) {
    EXPECT_TRUE(visitor.visit(*this).has_value());

    for (const auto& object : mTile.objects) {
      const testing::NiceMock<ObjectViewMock> object_view {object, this};
      EXPECT_TRUE(object_view.accept(visitor).has_value());
    }

    return std::expected<void, std::error_code> {};
  });
  ON_CALL(*this, get_parent_tileset).WillByDefault(ReturnRef(*mParentTileset));
  ON_CALL(*this, get_index).WillByDefault(Return(mTile.index));
  ON_CALL(*this, animation_frame_count).WillByDefault(Return(mTile.animation.size()));
  ON_CALL(*this, get_animation_frame).WillByDefault([this](std::size_t index) {
    const auto& frame = mTile.animation.at(index);
    return std::make_pair(frame.tile_index, frame.duration);
  });
  ON_CALL(*this, object_count).WillByDefault(Return(mTile.objects.size()));
  ON_CALL(*this, get_meta).WillByDefault(ReturnRef(mMeta));
}

TilesetViewMock::TilesetViewMock(ir::TilesetRef tileset_ref) :
  mTilesetRef {std::move(tileset_ref)},
  mMeta {mTilesetRef.tileset.meta}
{
  using testing::Return;
  using testing::ReturnRef;

  ON_CALL(*this, accept).WillByDefault([this](IDocumentVisitor& visitor) {
    EXPECT_TRUE(visitor.visit(*this).has_value());

    for (const auto& tile : mTilesetRef.tileset.tiles) {
      const testing::NiceMock<TileViewMock> tile_view {this, tile};
      EXPECT_TRUE(tile_view.accept(visitor).has_value());
    }

    return std::expected<void, std::error_code> {};
  });

  const auto& tileset = mTilesetRef.tileset;

  ON_CALL(*this, get_first_tile_id).WillByDefault(Return(mTilesetRef.first_tile_id));
  ON_CALL(*this, tile_count).WillByDefault(Return(tileset.tile_count));
  ON_CALL(*this, tile_definition_count).WillByDefault(Return(tileset.tiles.size()));
  ON_CALL(*this, column_count).WillByDefault(Return(tileset.column_count));
  ON_CALL(*this, get_tile_size).WillByDefault(Return(tileset.tile_size));
  ON_CALL(*this, get_image_size).WillByDefault(Return(tileset.image_size));
  ON_CALL(*this, get_image_path).WillByDefault(ReturnRef(tileset.image_path));
  ON_CALL(*this, get_filename).WillByDefault(Return(tileset.meta.name));
  ON_CALL(*this, get_meta).WillByDefault(ReturnRef(mMeta));
}

LayerViewMock::LayerViewMock(ir::Layer layer,
                             const ir::TileFormat& tile_format,
                             const LayerViewMock* parent_layer) :
  mLayer {std::move(layer)},
  mTileFormat {tile_format},
  mParentLayer {parent_layer},
  mMeta {mLayer.meta}
{
  using testing::NiceMock;
  using testing::Return;
  using testing::ReturnRef;

  ON_CALL(*this, accept).WillByDefault([this](IDocumentVisitor& visitor) {
    EXPECT_TRUE(visitor.visit(*this).has_value());

    for (const auto& sublayer : mLayer.layers) {
      const NiceMock<LayerViewMock> sublayer_view {sublayer, mTileFormat, this};
      EXPECT_TRUE(sublayer_view.accept(visitor).has_value());
    }

    for (const auto& subobject : mLayer.objects) {
      const NiceMock<ObjectViewMock> subobject_view {subobject, this};
      EXPECT_TRUE(subobject_view.accept(visitor).has_value());
    }

    return std::expected<void, std::error_code> {};
  });

  ON_CALL(*this, get_parent_layer).WillByDefault(Return(mParentLayer));
  ON_CALL(*this, get_id).WillByDefault(Return(mLayer.id));
  ON_CALL(*this, get_type).WillByDefault(Return(mLayer.type));
  ON_CALL(*this, get_opacity).WillByDefault(Return(mLayer.opacity));
  ON_CALL(*this, is_visible).WillByDefault(Return(mLayer.visible));

  ON_CALL(*this, layer_count).WillByDefault(Return(mLayer.layers.size()));
  ON_CALL(*this, object_count).WillByDefault(Return(mLayer.objects.size()));

  ON_CALL(*this, write_tile_bytes).WillByDefault([this](ByteStream& byte_stream) {
    byte_stream = to_byte_stream(mLayer.tiles);
  });

  ON_CALL(*this, get_extent).WillByDefault(Return(mLayer.extent));
  ON_CALL(*this, get_tile)
      .WillByDefault([this](const MatrixIndex& index) -> std::optional<TileID> {
        if (index.row >= 0 &&                  //
            index.col >= 0 &&                  //
            index.row < mLayer.extent.rows &&  //
            index.col < mLayer.extent.cols) {
          const auto u_row = saturate_cast<std::size_t>(index.row);
          const auto u_col = saturate_cast<std::size_t>(index.col);
          return mLayer.tiles.at(u_row).at(u_col);
        }

        return std::nullopt;
      });

  ON_CALL(*this, get_tile_encoding).WillByDefault(Return(mTileFormat.encoding));
  ON_CALL(*this, get_tile_compression).WillByDefault(Return(mTileFormat.compression));
  ON_CALL(*this, get_compression_level).WillByDefault(Return(mTileFormat.compression_level));

  ON_CALL(*this, get_meta).WillByDefault(ReturnRef(mMeta));
}

MapViewMock::MapViewMock(const ir::Map& map) :
  mMeta {map.meta}
{
  using testing::NiceMock;
  using testing::Return;
  using testing::ReturnRef;

  mTilesets.reserve(map.tilesets.size());
  mLayers.reserve(map.layers.size());

  for (const auto& tileset : map.tilesets) {
    auto tileset_view = std::make_unique<NiceMock<TilesetViewMock>>(tileset);
    mTilesets.push_back(std::move(tileset_view));
  }

  for (const auto& layer : map.layers) {
    auto layer_view =
        std::make_unique<NiceMock<LayerViewMock>>(layer, map.tile_format, nullptr);
    mLayers.push_back(std::move(layer_view));
  }

  ON_CALL(*this, accept).WillByDefault([this](IDocumentVisitor& visitor) {
    EXPECT_TRUE(visitor.visit(*this).has_value());

    for (const auto& tileset_view : mTilesets) {
      EXPECT_TRUE(tileset_view->accept(visitor).has_value());
    }

    for (const auto& layer_view : mLayers) {
      EXPECT_TRUE(layer_view->accept(visitor).has_value());
    }

    // TODO components

    return std::expected<void, std::error_code> {};
  });

  ON_CALL(*this, get_tile_size).WillByDefault(Return(map.tile_size));
  ON_CALL(*this, get_extent).WillByDefault(Return(map.extent));
  ON_CALL(*this, get_next_layer_id).WillByDefault(Return(map.next_layer_id));
  ON_CALL(*this, get_next_object_id).WillByDefault(Return(map.next_object_id));

  ON_CALL(*this, get_tile_encoding).WillByDefault(Return(map.tile_format.encoding));
  ON_CALL(*this, get_tile_compression).WillByDefault(Return(map.tile_format.compression));
  ON_CALL(*this, get_compression_level)
      .WillByDefault(Return(map.tile_format.compression_level));

  ON_CALL(*this, layer_count).WillByDefault(Return(map.layers.size()));
  ON_CALL(*this, tileset_count).WillByDefault(Return(map.tilesets.size()));
  ON_CALL(*this, component_count).WillByDefault(Return(map.components.size()));

  ON_CALL(*this, get_meta).WillByDefault(ReturnRef(mMeta));
}

}  // namespace tactile::test
