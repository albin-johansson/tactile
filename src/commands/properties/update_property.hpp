#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "maybe.hpp"
#include "property.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, property_manager)

namespace tactile::cmd {

class UpdateProperty final : public QUndoCommand
{
 public:
  UpdateProperty(core::property_manager* manager,
                 QString name,
                 core::property property);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::update_property);
  }

 private:
  core::property_manager* mManager{};
  QString mName;
  core::property mProperty;
  maybe<core::property> mPrevProperty;
};

}  // namespace tactile::cmd
