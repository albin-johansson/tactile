#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "maybe.hpp"
#include "property.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class remove_property final : public QUndoCommand
{
 public:
  explicit remove_property(core::property_manager* manager, QString name);

  void undo() override;

  void redo() override;

 private:
  core::property_manager* m_manager{};
  QString m_name;
  maybe<core::property> m_property;
};

}  // namespace tactile::cmd
