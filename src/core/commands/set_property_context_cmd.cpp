#include "set_property_context_cmd.hpp"

#include "core/document.hpp"
#include "core/properties/property_context.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

SetPropertyContextCmd::SetPropertyContextCmd(NotNull<IDocument*> document,
                                             IPropertyContext* context)
    : ACommand{"Set Property Context"}
    , mDocument{document}
    , mContext{context}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void SetPropertyContextCmd::Undo()
{
  mDocument->SetPropertyContext(mPrevious);
}

void SetPropertyContextCmd::Redo()
{
  mPrevious = mDocument->GetPropertyContext();
  mDocument->SetPropertyContext(mContext);
}

}  // namespace Tactile
