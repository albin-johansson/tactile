#pragma once

#include <vector_map.hpp>  // vector_map

#include "aliases/layer_id.hpp"
#include "layer.hpp"
#include "layer_delegate.hpp"

namespace Tactile {

class GroupLayer final : public ILayer
{
 public:
  using layer_storage = rune::vector_map<layer_id, SharedLayer>;
  using layer_pair = typename layer_storage::value_type;
  using const_iterator = typename layer_storage::const_iterator;

  GroupLayer();

  /// \name Layer API
  /// \{

  void SetVisible(bool visible) noexcept override;

  void SetOpacity(float opacity) override;

  void SetName(std::string name) override;

  [[nodiscard]] auto GetType() const -> LayerType override;

  [[nodiscard]] auto IsVisible() const -> bool override;

  [[nodiscard]] auto GetOpacity() const noexcept -> float override;

  [[nodiscard]] auto Clone() const -> SharedLayer override;

  /// \} End of layer API

  /// \name Property API
  /// \{

  void AddProperty(const std::string& name, PropertyType type) override;

  void AddProperty(const std::string& name, const Property& property) override;

  void RemoveProperty(std::string_view name) override;

  void RenameProperty(std::string_view oldName, const std::string& newName) override;

  void SetProperty(std::string_view name, const Property& property) override;

  void ChangePropertyType(std::string_view name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> std::string_view override;

  /// \} End of property API

  /// \name Group layer API
  /// \{

  // TODO will need "DragLayerCmd" or something to undo moving layers from/to groups

  void AddLayer(layer_id id, SharedLayer layer);

  [[nodiscard]] auto GetLayer(layer_id id) const -> const SharedLayer&;

  [[nodiscard]] auto ContainsLayer(layer_id id) const -> bool;

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mLayers.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return mLayers.end();
  }

  /// \} End of group layer API

 private:
  layer_storage mLayers;
  LayerDelegate mDelegate;
};

}  // namespace Tactile
