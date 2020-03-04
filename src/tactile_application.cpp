#include "tactile_application.h"

#include <QStyleFactory>
#include <QFile>

#include "tactile_editor.h"
#include "tactile_window.h"

namespace tactile {

TactileApplication::TactileApplication(int argc, char** argv)
    : QApplication{argc, argv},
      m_editor{std::make_unique<TactileEditor>()},
      m_window{std::make_unique<TactileWindow>()}
{
  setStyle(QStyleFactory::create("Fusion"));
  setApplicationVersion("0.1.0");
  setApplicationName("Tactile 2");

  QFile file{":/res/tactile_light.qss"};
  file.open(QFile::ReadOnly);
  QString StyleSheet = QLatin1String(file.readAll());
  setStyleSheet(StyleSheet);

  m_window->showMaximized();

  // TODO connect window paint events to editor paint
}



TactileApplication::~TactileApplication() noexcept = default;

}  // namespace tactile