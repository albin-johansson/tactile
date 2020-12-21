#pragma once

#include <QString>

#include "property.hpp"

namespace tactile::core {

class property_manager
{
 public:
  virtual ~property_manager() noexcept = default;

  virtual void add_property(const QString& name, property::type type) = 0;

  virtual void remove_property(const QString& name) = 0;

  virtual void rename_property(const QString& oldName,
                               const QString& newName) = 0;

  virtual void set_property(const QString& name,
                            const core::property& property) = 0;

  [[nodiscard]] virtual auto get_property(const QString& name) const
      -> const property& = 0;

  [[nodiscard]] virtual auto get_property(const QString& name) -> property& = 0;

  [[nodiscard]] virtual auto property_count() const -> int = 0;
};

}  // namespace tactile::core
