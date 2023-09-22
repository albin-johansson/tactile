// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/container/string_map.hpp"
#include "tactile/core/context/attribute.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Provides metadata associated with all meta context implementations.
 *
 * \see `IMetaContext`
 */
class Metadata final {
 public:
  /**
   * \brief Adds (or replaces) a property to the context.
   *
   * \param name the property name.
   *
   * \return the newly created attribute.
   */
  TACTILE_CORE_API auto add_property(String name) -> Attribute&;

  /**
   * \brief Removes a property from the context.
   *
   * \param name the property name.
   *
   * \return the remove property value; or nothing if it didn't exist.
   */
  TACTILE_CORE_API auto remove_property(StringView name) -> Maybe<Attribute>;

  /**
   * \brief Renames an existing property.
   *
   * \param current_name the current name of the property.
   * \param new_name     the new name of the property.
   *
   * \return true if a property was successfully renamed; false otherwise.
   */
  TACTILE_CORE_API auto rename_property(StringView current_name, String new_name) -> bool;

  /**
   * \brief Returns the property with a given name.
   *
   * \param name the property name.
   *
   * \return the property value.
   *
   * \throws Error if there is no matching property.
   */
  [[nodiscard]]
  TACTILE_CORE_API auto get_property(StringView name) -> Attribute&;

  /** \copydoc Metadata::get_property */
  [[nodiscard]]
  TACTILE_CORE_API auto get_property(StringView name) const -> const Attribute&;

  /**
   * \brief Sets the name associated with the object.
   *
   * \param name the new object name.
   */
  TACTILE_CORE_API void set_name(String name);

  /**
   * \brief Returns the current object name.
   *
   * \return the object name.
   */
  [[nodiscard]]
  TACTILE_CORE_API auto get_name() const -> const String&;

  /**
   * \brief Returns the number of attached properties.
   *
   * \return the property count.
   */
  [[nodiscard]]
  TACTILE_CORE_API auto property_count() const -> usize;

 private:
  String mName;
  StringMap<Attribute> mProperties;
};

}  // namespace tactile
