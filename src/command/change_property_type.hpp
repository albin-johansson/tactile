#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "property.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class change_property_type final : public QUndoCommand
{
 public:
  change_property_type(not_null<core::property_manager*> manager,
                       QString name,
                       core::property::type type1);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::change_property_type);
  }

 private:
  core::property_manager* m_manager{};
  QString m_name;
  core::property::type m_type;
  maybe<core::property> m_previousProperty;
};

}  // namespace tactile::cmd
