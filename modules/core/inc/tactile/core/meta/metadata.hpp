// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/string_map.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/meta/attribute.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Provides metadata associated with all meta context implementations.
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
   * \brief Adds (or replaces) a property to the context.
   *
   * \param  name the property name.
   * \return      the newly created attribute.
   */
  auto add_property(String name) -> Attribute&;

  /**
   * \brief Removes a property from the context.
   *
   * \param  name the property name.
   * \return      the remove property value; or nothing if it didn't exist.
   */
  auto remove_property(StringView name) -> Maybe<Attribute>;

  /**
   * \brief Renames an existing property.
   *
   * \param  current_name the current name of the property.
   * \param  new_name     the new name of the property.
   * \return              true if a property was successfully renamed; false otherwise.
   */
  auto rename_property(StringView current_name, String new_name) -> bool;

  /**
   * \brief Returns the property with a given name.
   *
   * \param  name the property name.
   * \return the property value.
   *
   * \throws Exception if there is no matching property.
   */
  [[nodiscard]]
  auto get_property(StringView name) -> Attribute&;

  /** \copydoc get_property() */
  [[nodiscard]]
  auto get_property(StringView name) const -> const Attribute&;

  /**
   * \brief Sets the name associated with the object.
   *
   * \param name the new object name.
   */
  void set_name(String name);

  /**
   * \brief Returns the associated UUID.
   *
   * \return a UUID.
   */
  [[nodiscard]]
  auto uuid() const -> const UUID&;

  /**
   * \brief Returns the current object name.
   *
   * \return the object name.
   */
  [[nodiscard]]
  auto name() const -> const String&;

  /**
   * \brief Returns the number of attached properties.
   *
   * \return the property count.
   */
  [[nodiscard]]
  auto property_count() const -> usize;

  /**
   * \brief Returns a clone of the metadata instance with another UUID.
   *
   * \return a metadata instance.
   */
  [[nodiscard]]
  auto clone() const -> Metadata;

 private:
  UUID mUUID {UUID::generate()};
  String mName;
  StringMap<Attribute> mProperties;
};

}  // namespace tactile
