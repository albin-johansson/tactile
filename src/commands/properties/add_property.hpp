#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand
#include <variant>       // variant, get_if, get

#include "command_id.hpp"
#include "not_null.hpp"
#include "property.hpp"
#include "property_manager.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

class AddProperty final : public QUndoCommand
{
 public:
  AddProperty(NotNull<core::IPropertyManager*> manager,
              QString name,
              const core::Property& property);

  AddProperty(NotNull<core::IPropertyManager*> manager,
              QString name,
              core::PropertyType type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::AddProperty);
  }

 private:
  using data_type = std::variant<core::PropertyType, core::Property>;

  core::IPropertyManager* mManager{};
  data_type mData;
  QString mName;

  AddProperty(NotNull<core::IPropertyManager*> manager,
              QString name,
              data_type data);
};

}  // namespace tactile::cmd
