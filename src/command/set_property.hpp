#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "maybe.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class set_property final : public QUndoCommand
{
 public:
  set_property(core::property_manager* manager,
               QString name,
               core::property property);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::set_property);
  }

 private:
  core::property_manager* m_manager{};
  QString m_name;
  core::property m_property;
  maybe<core::property> m_prevProperty;
};

}  // namespace tactile::cmd
