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
 * Represents a bundle of attributes that can be attached to meta contexts.
 */
class TACTILE_CORE_API Component final {
 public:
  using AttributeMap = StringMap<Attribute>;

  /**
   * Creates an empty component.
   *
   * \param id         The associated type identifier.
   * \param attributes The initial attributes.
   */
  explicit Component(ComponentID id, AttributeMap attributes = {});

  /**
   * Adds an empty string attribute in the component.
   *
   * \note
   *    This function will replace any existing attribute with the given name.
   *
   * \param name The attribute name.
   *
   * \return
   *    The new attribute.
   */
  auto add_attribute(String name) -> Attribute&;

  /**
   * Removes an attribute from the component.
   *
   * \param name The attribute name.
   *
   * \return
   *    The removed attribute.
   */
  auto remove_attribute(StringView name) -> Maybe<Attribute>;

  /**
   * Renames an existing attribute in the component.
   *
   * \note
   *    This function has no effect if the new name is used by another attribute.
   *
   * \param current_name The current name of the attribute.
   * \param new_name     The new name of the attribute.
   *
   * \return
   *    True if the attribute was successfully renamed; false otherwise.
   */
  auto rename_attribute(StringView current_name, String new_name) -> bool;

  /**
   * Returns an attribute in the component.
   *
   * \param name The attribute name.
   *
   * \return
   *    An attribute reference.
   */
  [[nodiscard]]
  auto get_attribute(StringView name) -> Attribute&;

  /** \copydoc get_attribute() */
  [[nodiscard]]
  auto get_attribute(StringView name) const -> const Attribute&;

  /**
   * Indicates whether the component features an attribute with a given name.
   *
   * \param name The attribute name to check.
   *
   * \return
   *    True if an attribute was found; false otherwise.
   */
  [[nodiscard]]
  auto has_attribute(StringView name) const -> bool;

  /**
   * Returns the total number of attributes in the component.
   *
   * \return
   *    An attribute count.
   */
  [[nodiscard]]
  auto attribute_count() const -> usize;

  /**
   * Returns the associated component type identifier.
   *
   * \return
   *    A component identifier.
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
