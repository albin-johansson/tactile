#pragma once

#include <QUndoCommand>

#include "command_id.hpp"
#include "core_fwd.hpp"
#include "not_null.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

class SetPropertyContext final : public QUndoCommand
{
 public:
  /**
   * \param document the associated map document.
   * \param context the property context that will be used, can be null to
   * indicate that the property context should be reset.
   */
  SetPropertyContext(not_null<core::MapDocument*> document,
                     core::IPropertyManager* context);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::SetPropertyContext);
  }

 private:
  core::MapDocument* mDocument{};
  core::IPropertyManager* mContext{};   ///< Nullable context that will be used.
  core::IPropertyManager* mPrevious{};  ///< Nullable previous context.
};

}  // namespace tactile::cmd
