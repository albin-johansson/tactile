#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class RenameProperty final : public QUndoCommand
{
 public:
  RenameProperty(core::IPropertyManager* manager,
                 QString oldName,
                 QString newName);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::RenameProperty);
  }

 private:
  core::IPropertyManager* mManager{};
  QString mOldName;
  QString mNewName;
};

}  // namespace tactile::cmd
