// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "document_delegate.hpp"

#include <utility>  // move

#include "tactile/core/debug/assert.hpp"

namespace tactile {

DocumentDelegate::DocumentDelegate(const UUID& root_context)
    : mContexts {root_context},
      mComponentIndex {std::make_shared<ComponentIndex>()}
{
}

void DocumentDelegate::set_component_index(Shared<ComponentIndex> index)
{
  mComponentIndex = std::move(index);
}

void DocumentDelegate::set_path(Path path)
{
  mPath = std::move(path);
}

auto DocumentDelegate::get_component_index() -> const Shared<ComponentIndex>&
{
  return mComponentIndex;
}

auto DocumentDelegate::view_component_index() const -> const ComponentIndex*
{
  return mComponentIndex.get();
}

auto DocumentDelegate::path() const -> const Path&
{
  TACTILE_ASSERT(mPath.has_value());
  return mPath.value();
}

auto DocumentDelegate::has_path() const -> bool
{
  return mPath.has_value();
}

auto DocumentDelegate::get_contexts() -> ContextManager&
{
  return mContexts;
}

auto DocumentDelegate::get_contexts() const -> const ContextManager&
{
  return mContexts;
}

auto DocumentDelegate::get_history() -> CommandStack&
{
  return mCommands;
}

auto DocumentDelegate::get_history() const -> const CommandStack&
{
  return mCommands;
}

auto DocumentDelegate::get_viewport() -> Viewport&
{
  return mViewport;
}

auto DocumentDelegate::get_viewport() const -> const Viewport&
{
  return mViewport;
}

}  // namespace tactile
