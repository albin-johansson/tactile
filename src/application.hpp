#pragma once

#include <centurion.hpp>  // window, gl_context

namespace tactile {

class Application final
{
 public:
  Application(cen::window&& window, cen::gl_context&& context);

  auto Run() -> int;

 private:
  cen::window mWindow;
  cen::gl_context mContext;

  void UpdateFrame();
};

}  // namespace tactile
