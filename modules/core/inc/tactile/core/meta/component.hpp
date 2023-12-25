#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/string_map.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/meta/attribute.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a bundle of attributes that can be attached to meta contexts.
 */
class TACTILE_CORE_API Component final {
 public:
  using AttributeMap = StringMap<Attribute>;

  /**
   * \brief Creates an empty component.
   *
   * \param id         the associated type identifier.
   * \param attributes the initial attributes.
   */
  explicit Component(ComponentID id, AttributeMap attributes = {});

  /**
   * \brief Adds an empty string attribute in the component.
   *
   * \note This function will replace any existing attribute with the given name.
   *
   * \param name the attribute name.
   *
   * \return the new attribute.
   */
  auto add_attribute(String name) -> Attribute&;

  /**
   * \brief Removes an attribute from the component.
   *
   * \param name the attribute name.
   *
   * \return the removed attribute.
   */
  auto remove_attribute(StringView name) -> Maybe<Attribute>;

  /**
   * \brief Renames an existing attribute in the component.
   *
   * \note This function has no effect if the new name is used by another attribute.
   *
   * \param current_name the current name of the attribute.
   * \param new_name     the new name of the attribute.
   *
   * \return true if the attribute was successfully renamed; false otherwise.
   */
  auto rename_attribute(StringView current_name, String new_name) -> bool;

  /**
   * \brief Returns an attribute in the component.
   *
   * \param name the attribute name.
   *
   * \return an attribute reference.
   */
  [[nodiscard]]
  auto get_attribute(StringView name) -> Attribute&;

  /**
   * \copydoc get_attribute()
   */
  [[nodiscard]]
  auto get_attribute(StringView name) const -> const Attribute&;

  /**
   * \brief Indicates whether the component features an attribute with a given name.
   *
   * \param name the attribute name to check.
   *
   * \return true if an attribute was found; false otherwise.
   */
  [[nodiscard]]
  auto has_attribute(StringView name) const -> bool;

  /**
   * \brief Returns the total number of attributes in the component.
   *
   * \return an attribute count.
   */
  [[nodiscard]]
  auto attribute_count() const -> usize;

  /**
   * \brief Returns the associated component type identifier.
   *
   * \return a component identifier.
   */
  [[nodiscard]]
  auto get_id() const -> ComponentID;

  [[nodiscard]]
  auto begin() const noexcept -> AttributeMap::const_iterator
  {
    return mAttributes.begin();
  }

  [[nodiscard]]
  auto end() const noexcept -> AttributeMap::const_iterator
  {
    return mAttributes.end();
  }

 private:
  ComponentID mComponentId;
  AttributeMap mAttributes;
};

}  // namespace tactile
