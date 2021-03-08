#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class rename_property final : public QUndoCommand
{
 public:
  rename_property(core::property_manager* manager,
                  QString oldName,
                  QString newName);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::rename_property);
  }

 private:
  core::property_manager* m_manager{};
  QString m_oldName;
  QString m_newName;
};

}  // namespace tactile::cmd
