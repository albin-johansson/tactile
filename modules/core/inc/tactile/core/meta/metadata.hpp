// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/meta/component.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/string_map.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/meta/attribute.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * Provides metadata associated with all meta context implementations.
 *
 * \see `IMetaContext`
 */
class TACTILE_CORE_API Metadata final {
 public:
  TACTILE_DELETE_COPY(Metadata);
  TACTILE_DEFAULT_MOVE(Metadata);

  Metadata() = default;

  ~Metadata() noexcept = default;

  /**
   * Adds (or replaces) a property to the context.
   *
   * \param name The property name.
   *
   * \return
   *    The newly created attribute.
   */
  auto add_property(String name) -> Attribute&;

  /**
   * Removes a property from the context.
   *
   * \param name The property name.
   *
   * \return
   *    The remove property value; or nothing if it didn't exist.
   */
  auto remove_property(StringView name) -> Maybe<Attribute>;

  /**
   * Renames an existing property.
   *
   * \param current_name The current name of the property.
   * \param new_name     The new name of the property.
   *
   * \return
   *    True if a property was successfully renamed; false otherwise.
   */
  auto rename_property(StringView current_name, String new_name) -> bool;

  /**
   * Indicates whether a property with the given name exists.
   *
   * \param name The name to look for.
   *
   * \return
   *    True if a property was found; false otherwise.
   */
  [[nodiscard]]
  auto has_property(StringView name) const -> bool;

  /**
   * Returns the property with a given name.
   *
   * \param name The property name.
   *
   * \return
   *    The property value.
   *
   * \throws Exception if there is no matching property.
   */
  [[nodiscard]]
  auto get_property(StringView name) -> Attribute&;

  /** \copydoc get_property() */
  [[nodiscard]]
  auto get_property(StringView name) const -> const Attribute&;

  /**
   * Sets the name associated with the object.
   *
   * \param name The new object name.
   */
  void set_name(String name);

  /**
   * Returns the associated UUID.
   *
   * \return
   *    A UUID.
   */
  [[nodiscard]]
  auto uuid() const -> const UUID&;

  /**
   * Returns the current object name.
   *
   * \return
   *    An object name.
   */
  [[nodiscard]]
  auto name() const -> const String&;

  /**
   * Returns the number of attached properties.
   *
   * \return
   *    A property count.
   */
  [[nodiscard]]
  auto property_count() const -> usize;

  [[nodiscard]]
  auto component_count() const -> usize;

  /**
   * Returns a clone of the metadata instance with another UUID.
   *
   * \return
   *    A metadata instance.
   */
  [[nodiscard]]
  auto clone() const -> Metadata;

  [[nodiscard]] auto property_begin() const { return mProperties.begin(); }
  [[nodiscard]] auto property_end() const { return mProperties.end(); }

  [[nodiscard]] auto component_begin() const { return mComponents.begin(); }
  [[nodiscard]] auto component_end() const { return mComponents.end(); }

 private:
  UUID mUUID {UUID::generate()};
  String mName {};
  StringMap<Attribute> mProperties {};
  Vector<Component> mComponents {};
};

}  // namespace tactile::core
