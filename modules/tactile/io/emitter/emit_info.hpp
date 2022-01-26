#pragma once

#include <filesystem>  // path
#include <functional>  // function
#include <string>      // string

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/components/component.hpp"

namespace tactile {

class attribute_value;

struct TilesetData final
{
  TilesetID id{};
  TileID first_tile{};

  int32 tile_width{};
  int32 tile_height{};
  int32 tile_count{};
  int32 column_count{};

  std::string name;

  std::filesystem::path image_path;
  int32 image_width{};
  int32 image_height{};
};

struct AnimationFrameData final
{
  TileID local_tile{};
  uint32 duration_ms{};
};

struct FancyTileData final
{
  TileID global_id{};
  TileID local_id{};
  ContextID context_id{};
};

struct LayerData final
{
  std::string name;
  LayerType type{};
  float opacity{};
  bool visible{};
};

struct ObjectData final
{
  ObjectID id{};

  float x{};
  float y{};
  float width{};
  float height{};

  std::string name;
  std::string tag;

  ObjectType type{};
  bool visible{};
};

class EmitInfo final {
 public:
  using component_def_attr_visitor = std::function<void(const std::string&,  //
                                                        const attribute_value&)>;

  using component_visitor = std::function<void(const std::string&,  //
                                               const component_attribute_map&)>;

  using tileset_visitor = std::function<void(const TilesetData&)>;
  using fancy_tile_visitor = std::function<void(const FancyTileData&)>;

  using object_visitor = std::function<void(const ObjectData&)>;
  using animation_frame_visitor = std::function<void(const AnimationFrameData&)>;

  using layer_visitor = std::function<void(LayerID)>;
  using layer_tile_visitor = std::function<void(usize, usize, TileID)>;

  using property_visitor =
      std::function<void(const std::string&, const attribute_value&)>;

  explicit EmitInfo(std::filesystem::path destination, const entt::registry& registry);

  /// \name Component definitions
  /// \{

  [[nodiscard]] auto component_def_count() const -> usize;

  [[nodiscard]] auto component_def_name(usize index) const -> const std::string&;

  [[nodiscard]] auto component_def_attr_count(usize index) const -> usize;

  void each_component_def_attrs(const component_def_attr_visitor& func,
                                usize definitionIndex) const;

  /// \} End of component definitions

  /// \name Tilesets
  /// \{

  [[nodiscard]] auto tileset_count() const -> usize;

  void each_tileset(const tileset_visitor& func) const;

  [[nodiscard]] auto tileset_context(TilesetID id) const -> ContextID;

  [[nodiscard]] auto tileset_fancy_tile_count(TilesetID id) const -> usize;

  void each_tileset_fancy_tile(TilesetID id, const fancy_tile_visitor& func) const;

  [[nodiscard]] auto fancy_tile_object_count(TileID id) const -> usize;

  void each_fancy_tile_object(TileID id, const object_visitor& func) const;

  [[nodiscard]] auto fancy_tile_animation_frame_count(TileID id) const -> usize;

  void each_fancy_tile_animation_frame(TileID id,
                                       const animation_frame_visitor& func) const;

  /// \} End of tilesets

  /// \name Layers
  /// \{

  [[nodiscard]] auto layer_count(Maybe<LayerID> id = {}) const -> usize;

  void each_layer(Maybe<LayerID> parentId, const layer_visitor& func) const;

  [[nodiscard]] auto layer_data(LayerID id) const -> LayerData;

  void each_layer_tile(LayerID id, const layer_tile_visitor& func) const;

  [[nodiscard]] auto layer_object_count(LayerID id) const -> usize;

  void each_layer_object(LayerID id, const object_visitor& func) const;

  [[nodiscard]] auto layer_context(LayerID id) const -> ContextID;

  [[nodiscard]] auto object_context(ObjectID id) const -> ContextID;

  /// \} End of layers

  /// \name Components
  /// \{

  [[nodiscard]] auto component_count(ContextID id) const -> usize;

  void each_component(ContextID id, const component_visitor& func) const;

  /// \} End of components

  /// \name Properties
  /// \{

  [[nodiscard]] auto property_count(ContextID id) const -> usize;

  void each_property(ContextID id, const property_visitor& func) const;

  /// \} End of properties

  /// \name Map information
  /// \{

  [[nodiscard]] auto root_context() const -> ContextID;

  [[nodiscard]] auto tile_width() const -> int32;

  [[nodiscard]] auto tile_height() const -> int32;

  [[nodiscard]] auto next_layer_id() const -> LayerID;

  [[nodiscard]] auto next_object_id() const -> ObjectID;

  [[nodiscard]] auto row_count() const -> usize;

  [[nodiscard]] auto column_count() const -> usize;

  /// \} End of map information

  /// \name Target file destination
  /// \{

  [[nodiscard]] auto destination_file() const -> const std::filesystem::path&;

  [[nodiscard]] auto destination_dir() const -> const std::filesystem::path&;

  /// \} End of target file destination

 private:
  std::filesystem::path mDestinationFile;
  std::filesystem::path mDestinationDir;
  const entt::registry* mRegistry;

  [[nodiscard]] auto to_tileset_entity(TilesetID id) const -> entt::entity;

  [[nodiscard]] auto to_tile_entity(TileID id) const -> entt::entity;

  [[nodiscard]] auto to_layer_entity(LayerID id) const -> entt::entity;

  [[nodiscard]] auto to_object_entity(ObjectID id) const -> entt::entity;
};

}  // namespace tactile