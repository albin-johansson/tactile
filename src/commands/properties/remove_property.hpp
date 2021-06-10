#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "maybe.hpp"
#include "property.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, property_manager)

namespace tactile::cmd {

class RemoveProperty final : public QUndoCommand
{
 public:
  explicit RemoveProperty(core::property_manager* manager, QString name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::remove_property);
  }

 private:
  core::property_manager* mManager{};
  QString mName;
  maybe<core::property> mProperty;
};

}  // namespace tactile::cmd
