#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand
#include <variant>       // variant, get_if, get

#include "command_id.hpp"
#include "not_null.hpp"
#include "property.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class add_property final : public QUndoCommand
{
  using data_type = std::variant<core::property_type, core::property>;

 public:
  add_property(not_null<core::property_manager*> manager,
               QString name,
               const core::property& property);

  add_property(not_null<core::property_manager*> manager,
               QString name,
               core::property_type type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::add_property);
  }

 private:
  core::property_manager* m_manager{};
  data_type m_data;
  QString m_name;

  add_property(not_null<core::property_manager*> manager,
               QString name,
               data_type data);
};

}  // namespace tactile::cmd
