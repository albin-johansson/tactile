#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "maybe.hpp"
#include "property.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, property_manager)

namespace tactile::cmd {

class update_property final : public QUndoCommand
{
 public:
  update_property(core::property_manager* manager,
                  QString name,
                  core::property property);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::update_property);
  }

 private:
  core::property_manager* m_manager{};
  QString m_name;
  core::property m_property;
  maybe<core::property> m_prevProperty;
};

}  // namespace tactile::cmd
