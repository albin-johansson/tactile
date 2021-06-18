#include "set_property_context.hpp"

#include "document.hpp"
#include "document_delegate.hpp"
#include "map_document.hpp"
#include "property_manager.hpp"
#include "verify_not_null.hpp"

namespace tactile::cmd {

SetPropertyContext::SetPropertyContext(not_null<core::MapDocument*> document,
                                       core::IPropertyManager* context)
    : QUndoCommand{QStringLiteral(u"Set Property Context")}
    , mDocument{VerifyNotNull(document)}
    , mContext{context}
{}

void SetPropertyContext::undo()
{
  QUndoCommand::undo();

  auto& delegate = mDocument->GetDelegate();
  if (mPrevious)
  {
    qDebug("SetPropertyContext::undo: Setting previous context as active");
    delegate.SetPropertyContext(mPrevious);
    emit mDocument->S_UpdatedPropertyContext(mPrevious, delegate.GetName());
  }
  else
  {
    qDebug("SetPropertyContext::undo: Resetting property context");
    delegate.ResetPropertyContext();
    emit mDocument->S_UpdatedPropertyContext(nullptr, delegate.GetName());
  }
}

void SetPropertyContext::redo()
{
  QUndoCommand::redo();

  mPrevious = mDocument->GetPropertyContext();

  auto& delegate = mDocument->GetDelegate();
  if (mContext)
  {
    qDebug("SetPropertyContext::redo: Setting non-null property context");
    delegate.SetPropertyContext(mContext);
  }
  else
  {
    qDebug("SetPropertyContext::redo: Resetting property context");
    delegate.ResetPropertyContext();
  }

  emit mDocument->S_UpdatedPropertyContext(mContext, delegate.GetName());
}

}  // namespace tactile::cmd
