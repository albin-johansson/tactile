#pragma once
#include <QApplication>

#include "smart_pointers.h"

namespace tactile {

class TactileWindow;
class TactileEditor;

class TactileApplication final : public QApplication {
 public:
  TactileApplication(int argc, char** argv);

  ~TactileApplication() noexcept override;

 private:
  UniquePtr<TactileWindow> m_window;
  UniquePtr<TactileEditor> m_editor;
};

}  // namespace tactile
