#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "fwd.hpp"
#include "maybe.hpp"
#include "property.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

class UpdateProperty final : public QUndoCommand
{
 public:
  UpdateProperty(core::IPropertyManager* manager,
                 QString name,
                 core::Property property);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::UpdateProperty);
  }

 private:
  core::IPropertyManager* mManager{};
  QString mName;
  core::Property mProperty;
  Maybe<core::Property> mPrevProperty;
};

}  // namespace tactile::cmd
