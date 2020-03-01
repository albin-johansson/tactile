#include "tactile_application.h"

#include <QStyleFactory>

namespace tactile {

TactileApplication::TactileApplication(int argc, char** argv) : QApplication{argc, argv} {
  setStyle(QStyleFactory::create("Fusion"));
  setApplicationVersion("0.1.0");
  setApplicationName("Tactile 2");
}

}