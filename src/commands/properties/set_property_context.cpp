#include "set_property_context.hpp"

#include "document.hpp"
#include "document_delegate.hpp"
#include "map_document.hpp"
#include "property_manager.hpp"
#include "verify_not_null.hpp"

namespace tactile::cmd {

SetPropertyContext::SetPropertyContext(NotNull<core::MapDocument*> document,
                                       core::IPropertyManager* context)
    : QUndoCommand{QStringLiteral(u"Set Property Context")}
    , mDocument{VerifyNotNull(document)}
    , mContext{context}
{}

void SetPropertyContext::undo()
{
  QUndoCommand::undo();

  auto& delegate = mDocument->GetDelegate();
  delegate.SetPropertyContext(mPrevious);

  emit mDocument->S_UpdatedPropertyContext(mPrevious, delegate.GetName());
}

void SetPropertyContext::redo()
{
  QUndoCommand::redo();

  mPrevious = mDocument->GetPropertyContext();

  auto& delegate = mDocument->GetDelegate();
  delegate.SetPropertyContext(mContext);

  emit mDocument->S_UpdatedPropertyContext(mContext, delegate.GetName());
}

}  // namespace tactile::cmd
