#pragma once

#include "aliases/maybe.hpp"
#include "aliases/unique.hpp"
#include "core/properties/property_context.hpp"
#include "core/properties/property_delegate.hpp"
#include "tile_animation.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

/**
 * \class FancyTile
 *
 * \brief Provides additional information about tiles in tilesets, e.g. animations.
 *
 * \details A `Tileset` might contain no "fancy" tiles at all, only tiles with
 * additional meta-information such as properties or animations are considered to be
 * fancy tiles.
 *
 * \see `Tileset`
 *
 * \since 0.1.0
 */
class FancyTile final : public IPropertyContext
{
 public:
  FancyTile();

  /**
   * \brief Sets the animation for the tile.
   *
   * \param animation the animation that will be associated with the tile.
   *
   * \since 0.1.0
   */
  void SetAnimation(TileAnimation animation);

  /**
   * \brief Returns the associated animation.
   *
   * \return the associated animation; `nothing` if there is no such animation.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto GetAnimation() -> Maybe<TileAnimation>&;

  /// \copydoc GetAnimation()
  [[nodiscard]] auto GetAnimation() const -> const Maybe<TileAnimation>&;

  /// \name Property API
  /// \{

  void AddProperty(std::string name, PropertyType type) override;

  void AddProperty(std::string name, const Property& property) override;

  void RemoveProperty(std::string_view name) override;

  void RenameProperty(std::string_view oldName, std::string newName) override;

  void SetProperty(std::string_view name, const Property& property) override;

  void ChangePropertyType(std::string name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> const std::string& override;

  /// \} End of property API

 private:
  Maybe<TileAnimation> mAnimation;
  Unique<PropertyDelegate> mProperties;
};

/// \} End of group core

}  // namespace Tactile
