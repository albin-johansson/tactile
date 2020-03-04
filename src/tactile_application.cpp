#include "tactile_application.h"

#include <QStyleFactory>

#include "tactile_editor.h"

namespace tactile {

TactileApplication::TactileApplication(int argc, char** argv)
    : QApplication{argc, argv}
{
  m_editor = std::make_unique<TactileEditor>();

  setStyle(QStyleFactory::create("Fusion"));
  setApplicationVersion("0.1.0");
  setApplicationName("Tactile 2");  
}

TactileApplication::~TactileApplication() noexcept = default;

}  // namespace tactile