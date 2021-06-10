#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class RenameProperty final : public QUndoCommand
{
 public:
  RenameProperty(core::property_manager* manager,
                 QString oldName,
                 QString newName);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::rename_property);
  }

 private:
  core::property_manager* mManager{};
  QString mOldName;
  QString mNewName;
};

}  // namespace tactile::cmd
