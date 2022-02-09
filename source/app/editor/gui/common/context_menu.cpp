#include "context_menu.hpp"

#include "editor/gui/scoped.hpp"
#include "misc/throw.hpp"

namespace tactile {

context_menu::context_menu(const char* name) : mName{name}
{
  if (!mName) {
    throw_traced(tactile_error{"Invalid null context menu name!"});
  }
}

void context_menu::update(const document_model& model, entt::dispatcher& dispatcher)
{
  if (scoped::popup popup{mName}; popup.is_open()) {
    on_update(model, dispatcher);
  }

  if (mShow) {
    ImGui::OpenPopup(mName);
    mShow = false;
  }
}

void context_menu::show()
{
  mShow = true;
}

}  // namespace tactile