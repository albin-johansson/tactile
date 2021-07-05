#pragma once

#include "aliases/not_null.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"

namespace Tactile {

class IDocument;
class IPropertyContext;

class SetPropertyContextCmd final : public ACommand
{
 public:
  SetPropertyContextCmd(NotNull<IDocument*> document, IPropertyContext* context);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetPropertyContext;
  }

 private:
  IDocument* mDocument{};
  IPropertyContext* mContext{};
  IPropertyContext* mPrevious{};
};

}  // namespace Tactile
