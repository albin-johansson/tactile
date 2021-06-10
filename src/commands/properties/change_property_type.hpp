#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "property.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class ChangePropertyType final : public QUndoCommand
{
 public:
  ChangePropertyType(not_null<core::property_manager*> manager,
                     QString name,
                     core::property_type type1);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::change_property_type);
  }

 private:
  core::property_manager* mManager{};
  QString mName;
  core::property_type mType;
  maybe<core::property> mPreviousProperty;
};

}  // namespace tactile::cmd
