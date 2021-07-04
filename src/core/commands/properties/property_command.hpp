#pragma once

#include <string>  // string

#include "aliases/not_null.hpp"
#include "core/commands/command.hpp"

namespace Tactile {

class IPropertyContext;

class APropertyCommand : public ACommand
{
 public:
  APropertyCommand(NotNull<IPropertyContext*> context, std::string name);

 protected:
  [[nodiscard]] auto GetContext() -> IPropertyContext&;

 private:
  IPropertyContext* mContext{};
};

}  // namespace Tactile
