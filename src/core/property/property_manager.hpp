#pragma once

#include <QString>

#include "property.hpp"

namespace tactile::core {

class property_manager
{
 public:
  virtual ~property_manager() noexcept = default;

  /**
   * \brief Adds a property with the specified name and type.
   *
   * \pre there must not be a property associated with `name` when this function
   * is invoked.
   *
   * \details The added property will use its default value.
   *
   * \param name the name that will be associated with the property.
   * \param type the type of the property.
   *
   * \since 0.2.0
   */
  virtual void add_property(const QString& name, property::type type) = 0;

  /**
   * \brief Removes the property associated with the specified name.
   *
   * \pre there must be a property associated with `name` when this function is
   * invoked.
   *
   * \param name the name of the property that will be removed.
   *
   * \since 0.2.0
   */
  virtual void remove_property(const QString& name) = 0;

  /**
   * \brief Changes the name associated with a property.
   *
   * \pre there must be a property associated with `oldName` when this function
   * is invoked.
   * \pre there must not be a property associated with `newName` when this
   * function is invoked.
   *
   * \param oldName the name that used to be associated with the property.
   * \param newName the new name that will be associated with the property.
   *
   * \since 0.2.0
   */
  virtual void rename_property(const QString& oldName,
                               const QString& newName) = 0;

  /**
   * \brief Sets the value of a property.
   *
   * \pre there must be a property associated with `name` when this function
   * is invoked.
   * \pre `property` must be of the same type as the previous value.
   *
   * \param name the name of the property that will be changed.
   * \param property the new value of the property.
   *
   * \since 0.2.0
   */
  virtual void set_property(const QString& name,
                            const core::property& property) = 0;

  /**
   * \brief Returns the property associated with the specified name.
   *
   * \note This function throws if there is no property associated with the
   * specified name.
   *
   * \param name the name associated with the desired property.
   *
   * \return the property associated with `name`.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto get_property(const QString& name) const
      -> const property& = 0;

  /**
   * \copydoc property_manager::get_property
   */
  [[nodiscard]] virtual auto get_property(const QString& name) -> property& = 0;

  /**
   * \brief Returns the amount of properties handled by the manager.
   *
   * \return the current amount of properties.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto property_count() const noexcept -> int = 0;
};

}  // namespace tactile::core
