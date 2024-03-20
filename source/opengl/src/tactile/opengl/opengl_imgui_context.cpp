// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_imgui_context.hpp"

#include <cstdlib>  // malloc, free
#include <utility>  // exchange

#include <imgui.h>

#include "tactile/opengl/opengl_error.hpp"

namespace tactile {

auto OpenGLImGuiContext::make() -> Result<OpenGLImGuiContext>
{
  ImGui::SetAllocatorFunctions(
      [](const usize size, void*) { return std::malloc(size); },  // NOLINT(*-no-malloc)
      [](void* ptr, void*) { std::free(ptr); });                  // NOLINT(*-no-malloc)

  if (auto* imgui = ImGui::CreateContext()) {
    return OpenGLImGuiContext {imgui};
  }

  return unexpected(make_error(OpenGLError::kImGuiError));
}

OpenGLImGuiContext::OpenGLImGuiContext(ImGuiContext* context) noexcept
  : mContext {context}
{}

OpenGLImGuiContext::OpenGLImGuiContext(OpenGLImGuiContext&& other) noexcept
  : mContext {std::exchange(other.mContext, nullptr)}
{}

OpenGLImGuiContext::~OpenGLImGuiContext() noexcept
{
  _dispose();
}

void OpenGLImGuiContext::_dispose() noexcept
{
  if (mContext) {
    ImGui::DestroyContext(mContext);
    mContext = nullptr;
  }
}

auto OpenGLImGuiContext::operator=(OpenGLImGuiContext&& other) noexcept
    -> OpenGLImGuiContext&
{
  if (this != &other) {
    _dispose();
    mContext = std::exchange(other.mContext, nullptr);
  }

  return *this;
}

auto OpenGLImGuiContext::get() noexcept -> ImGuiContext*
{
  return mContext;
}

}  // namespace tactile
